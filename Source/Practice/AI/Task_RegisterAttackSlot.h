// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_RegisterAttackSlot.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UTask_RegisterAttackSlot : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UTask_RegisterAttackSlot();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
