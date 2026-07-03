// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Task_UseSkill.h"
#include "AIController.h"
#include "Monster.h"
#include "SkillComponent.h"
#include "SkillDataMonster.h"

#include "BehaviorTree/BlackboardComponent.h"

UTask_UseSkill::UTask_UseSkill()
{
	// Task를 1개로 돌려쓰는 구조가 아니라, 사용하는 각 컨트롤러마다 1 객체씩 부여
	// 이러면 Task의 멤버변수를 활용해도 문제 없음.
	bCreateNodeInstance = false;
	
	// 매 프레임마다 TickTask를 호출받을 지 설정
	bNotifyTick = true;
}

EBTNodeResult::Type UTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//UseSkillData* pData = (UseSkillData*)NodeMemory;

	// Task를 실행중인 Controller를 가져온다.
	AAIController* pController = OwnerComp.GetAIOwner();
	AMonster* pMonster = Cast<AMonster>(pController->GetPawn());
	if (pMonster == nullptr)
		return EBTNodeResult::Failed;

	USkillComponent* pSkillCom = pMonster->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return EBTNodeResult::Failed;

	if (APawn* pTargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(m_Target.SelectedKeyName)))
		pSkillCom->SetTargetPawn(pTargetPawn);

	bool bSuccess = pSkillCom->TryExecuteSkill((int32)ESkillSlot::Skill_1);

	if (bSuccess)
	{
		// 스킬이 종료될 때 호출받을 Delegate 를 등록.
		pSkillCom->m_SkillEndDelegate.AddUObject(this, &UTask_UseSkill::OnSkillEnd, &OwnerComp);

		// 멀티캐스트 - 다른 클라이언트들에게도 상황을 전달.
		pSkillCom->Multicast_SkillExecute((int32)ESkillSlot::Skill_1, 0);

		// 스킬 시전 시간 동안 Skill Task가 진행중임을 리턴한다.
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UTask_UseSkill::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	*reinterpret_cast<float*>(NodeMemory) = 0.f;
	// FinishLatentTask 함수 활용하기?
	// 매 프레임 물으면 연산 낭비. => Delegate를 활용하기.

	// 타겟을 향해서 추적회전을 시도한다.
	APawn* SkillUser = OwnerComp.GetAIOwner()->GetPawn();

	if (SkillUser == nullptr)
		return;

	USkillComponent* pSkillCom = SkillUser->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	const USkillDataMonster* pSkill = Cast<USkillDataMonster>(pSkillCom->GetCurSkillInfo().Get());
	if (pSkill == nullptr)
		return;

	// 기존 방식으론 멤버로 m_elapsed, m_duration 등의 멤버함수를 두고 객체마다 시간을 따로 계산해야한다.
	// 해당 Task는 객체들이 공유하는 데이터이기 때문.
	// param 2의 NodeMemory는 이를 가능하게 한다.
	// 생성자의 bCreateNodeInstance를 false로 꺼뒀기 때문에, 기존 방식 대신 param2를 활용할 수 있다.

	if (*reinterpret_cast<float*>(NodeMemory) < pSkill->TraceRotateTime)
	{
		*reinterpret_cast<float*>(NodeMemory) += DeltaSeconds;
		
		AActor* pTargetActor = 
			Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(m_Target.SelectedKeyName));
		if (pTargetActor == nullptr)
			return;

		FVector vDir = pTargetActor->GetActorLocation() - SkillUser->GetActorLocation();
		vDir.Z = 0.f;
		vDir.Normalize();

		FRotator TargetRot = FRotationMatrix::MakeFromX(vDir).Rotator();

		FRotator Rotate = FMath::RInterpConstantTo(SkillUser->GetActorRotation(),
			TargetRot, DeltaSeconds,
			pSkill->TraceRotateSpeed);

		SkillUser->SetActorRotation(Rotate);
	}
}

void UTask_UseSkill::OnSkillEnd(AActor* _SkillUser, UBehaviorTreeComponent* _BTCom)
{
	if (_SkillUser && _BTCom)
	{
		USkillComponent* pSkillCom = _SkillUser->GetComponentByClass<USkillComponent>();
		pSkillCom->m_SkillEndDelegate.RemoveAll(this);
	}

	FinishLatentTask(*_BTCom, EBTNodeResult::Succeeded);
}
