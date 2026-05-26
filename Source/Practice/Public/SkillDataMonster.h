// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillDataBase.h"
#include "SkillDataMonster.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API USkillDataMonster : public USkillDataBase
{
	GENERATED_BODY()

public:
	virtual bool CanUseSkill_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom) override;
	virtual void OnExecuteSkill_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom) override;
	virtual void OnEndSkill_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Mon")
	float TraceRotateSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Mon")
	float TraceRotateTime;
};
