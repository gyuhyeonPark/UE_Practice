// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UTask_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_Patrol();

protected:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

	virtual void TickTask(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds) override;

protected:
	// Blackboard���� Target�� �������� ���� Key
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector m_Target;

	// �÷��̾�� ������ �Ÿ�
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float m_PatrolRadius = 500.f;

	// �� ���� ȸ���� ����
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float m_PatrolAngle = 45.f;
};
