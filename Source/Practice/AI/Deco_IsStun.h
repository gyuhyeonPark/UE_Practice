// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Deco_IsStun.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UDeco_IsStun : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UDeco_IsStun();

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMem) const override;
};
