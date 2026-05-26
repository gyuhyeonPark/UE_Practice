// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Serv_SelectTarget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UServ_SelectTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UServ_SelectTarget();

public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector	m_Target;
};
