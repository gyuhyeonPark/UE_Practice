// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Task_Patrol.h"
#include "NPC.h"
#include "MonsterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UTask_Patrol::UTask_Patrol()
{
	NodeName = TEXT("Patrol");

	bNotifyTick = true;
}

EBTNodeResult::Type UTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
		return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard =
		OwnerComp.GetBlackboardComponent();

	if (!Blackboard)
		return EBTNodeResult::Failed;

	AActor* pTargetActor =
		Cast<AActor>(
			Blackboard->GetValueAsObject(
				m_Target.SelectedKeyName));

	if (!pTargetActor)
		return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();

	if (!Pawn)
		return EBTNodeResult::Failed;

	const FVector TargetLocation =
		pTargetActor->GetActorLocation();

	const FVector MonsterLocation =
		Pawn->GetActorLocation();

	// �÷��̾� -> ���� ����
	FVector Direction =
		MonsterLocation - TargetLocation;

	Direction.Z = 0.f;

	if (Direction.IsNearlyZero())
	{
		return EBTNodeResult::Failed;
	}

	Direction.Normalize();

	// ���� ����
	float CurrentAngle =
		FMath::Atan2(Direction.Y, Direction.X);

	// ���� ��ġ�� ����
	CurrentAngle +=
		FMath::DegreesToRadians(m_PatrolAngle);

	FVector NextDirection;

	NextDirection.X = FMath::Cos(CurrentAngle);
	NextDirection.Y = FMath::Sin(CurrentAngle);
	NextDirection.Z = 0.f;

	// Target�� �߽����� ���� ��ġ ���
	const FVector Destination =
		TargetLocation +
		NextDirection * m_PatrolRadius;

	// �̵�
	AIController->MoveToLocation(Destination);

	UE_LOG(LogTemp, Warning, TEXT("!!!Patrol EXECUTING!!!"));

	return EBTNodeResult::InProgress;
}

void UTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController =
		OwnerComp.GetAIOwner();

	if (!AIController)
	{
		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Failed);

		return;
	}

	UBlackboardComponent* Blackboard =
		OwnerComp.GetBlackboardComponent();

	if (!Blackboard)
	{
		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Failed);

		return;
	}

	AActor* pTargetActor =
		Cast<AActor>(
			Blackboard->GetValueAsObject(
				m_Target.SelectedKeyName));

	APawn* Pawn = AIController->GetPawn();

	if (!pTargetActor || !Pawn)
	{
		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Failed);

		return;
	}

	// Target�� �ٶ󺻴�.
	FVector Direction =
		pTargetActor->GetActorLocation() -
		Pawn->GetActorLocation();

	Direction.Z = 0.f;

	if (!Direction.IsNearlyZero())
	{
		Pawn->SetActorRotation(
			Direction.Rotation());
	}

	// �������� �����ϸ� Task ����
	if (AIController->GetMoveStatus() ==
		EPathFollowingStatus::Idle)
	{
		FinishLatentTask(
			OwnerComp,
			EBTNodeResult::Succeeded);
	}
}
