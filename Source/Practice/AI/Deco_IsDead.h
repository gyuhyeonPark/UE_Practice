// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Deco_IsDead.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UDeco_IsDead : public UBTDecorator
{
	GENERATED_BODY()

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMem) const override;
};
