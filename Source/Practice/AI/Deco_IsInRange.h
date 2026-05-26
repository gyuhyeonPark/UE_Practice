// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Deco_IsInRange.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UDeco_IsInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UDeco_IsInRange();

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMem) const override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector m_Target;


};
