// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatComponent.h"
#include "MonsterStatComponent.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UMonsterStatComponent : public UStatComponent
{
	GENERATED_BODY()

public:
	UMonsterStatComponent();

public:
	virtual void BeginPlay() override;
	virtual void InitStat() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (RequiredAssetDataTags = "RowStructure=/Script/Practice.MonsterStatData"))
	UDataTable* m_Table;
};
