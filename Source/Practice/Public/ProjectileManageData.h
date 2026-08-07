// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileManageData.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UProjectileManageData : public UDataAsset
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere)
    float RangeRadius;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class AWarningSign> WarningSignClass;
};
