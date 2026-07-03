// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillDataBase.h"
#include "SkillDataPlayer.generated.h"

UENUM(BlueprintType)
enum class EBModeTag : uint8
{
	NONE,
	CHARGE,
	SWING,
};

/**
 * 
 */
UCLASS()
class PRACTICE_API USkillDataPlayer : public USkillDataBase
{
	GENERATED_BODY()
	
public:
	virtual bool CanUseSkill_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom) override;
	virtual void OnExecuteSkill_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom) override;
	virtual void OnEndSkill_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom) override;
};
