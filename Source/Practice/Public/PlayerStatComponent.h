// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "PlayerStatComponent.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UPlayerStatComponent : public UStatComponent
{
	GENERATED_BODY()
	
public:
	UPlayerStatComponent();
	virtual void BeginPlay() override;

public:
	virtual void InitStat() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (RequiredAssetDataTags = "RowStructure=/Script/Practice.PlayerStatData"))
	UDataTable* m_Table;

};
