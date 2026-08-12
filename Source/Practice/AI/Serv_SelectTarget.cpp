// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Serv_SelectTarget.h"
#include "MonsterController.h"
#include "Monster.h"
#include "BehaviorTree/BlackboardComponent.h"

UServ_SelectTarget::UServ_SelectTarget()
{
	Interval = 0.3f;
	RandomDeviation = 0.1f;
}

void UServ_SelectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AMonsterController* pController = Cast<AMonsterController>(OwnerComp.GetAIOwner());
	if (!pController)
		return;

	AMonster* pMonster = Cast<AMonster>(pController->GetPawn());
	if (!pMonster)
		return;

	// 인지 범위를 벗어난 대상이 일정 시간 지나면 인지 목록에서 제거
	pController->ExpireSensedTarget(3.f);

	float MaxAggro = -1.f;
	AActor* pBestTarget = nullptr;
	FVector Pos;

	// 컨트롤러가 인지한 대상 중에 가장 적절한 대상을 골라 블랙보드 Target에 업로드
	for (const FSensedTargetInfo& Info : pController->GetSensedTargets())
	{
		if (!Info.Target.IsValid())
			continue;

		AActor* Target = Info.Target.Get();
		// 자기 자신은 타겟 후보에서 제외
		if (Target == pMonster)
			continue;

		// 최우선 순위는 어그로 유지
		if (MaxAggro < Info.AggroValue)
		{
			MaxAggro = Info.AggroValue;

			pBestTarget = Info.Target.Get();

			Pos = Info.Target->GetActorLocation();
		}
		// 차선, 어그로가 동일한 경우
		else if (MaxAggro == Info.AggroValue)
		{
			float DistOrgin = FVector::Dist(pMonster->GetActorLocation(), Pos);
			float DistNew = FVector::Dist(pMonster->GetActorLocation(), Info.Target->GetActorLocation());

			// 어그로가 동일하지만, 이번 대상이 거리가 더 가까우면 BestTarget을 교체
			if (DistNew < DistOrgin)
			{
				pBestTarget = Info.Target.Get();
				Pos = Info.Target->GetActorLocation();
			}
		}
	}

	UBlackboardComponent* pBBCom = OwnerComp.GetBlackboardComponent();
	if (pBBCom == nullptr)
		return;

	pBBCom->SetValueAsObject(m_Target.SelectedKeyName, pBestTarget);
}
