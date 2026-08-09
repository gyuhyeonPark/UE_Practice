// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Serv_RotateToTarget.h"

#include "MonsterController.h"
#include "Monster.h"
#include "SkillComponent.h"
#include "SkillDataMonster.h"

#include "BehaviorTree/BlackboardComponent.h"

UServ_RotateToTarget::UServ_RotateToTarget()
{
	// 서비스의 Tick 함수가 호출될 간격. 0으로 설정하면 매 프레임마다 Tick 호출
	Interval = 0.f;

	// 랜덤 편차, Interval에 +- 랜덤 부여.
	RandomDeviation = 0.f;
}

void UServ_RotateToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// 타겟을 향해서 추적회전을 시도한다.
	APawn* pMonster = OwnerComp.GetAIOwner()->GetPawn();

	if (pMonster == nullptr)
		return;

	AMonster* pMon = Cast<AMonster>(pMonster);
	if (pMon == nullptr)
		return;
	if (pMon->IsDead())
		return;

	AActor* pTargetActor =
		Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(m_Target.SelectedKeyName));
	if (pTargetActor == nullptr)
		return;

	FVector vDir = pTargetActor->GetActorLocation() - pMonster->GetActorLocation();
	vDir.Z = 0.f;
	vDir.Normalize();

	FRotator TargetRot = FRotationMatrix::MakeFromX(vDir).Rotator();

	FRotator Rotate = FMath::RInterpConstantTo(pMonster->GetActorRotation(),
		TargetRot, DeltaSeconds,
		m_RotateSpeed);

	pMonster->SetActorRotation(Rotate);
}
