// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillComponent.h"
#include "PlayerSkillComponent.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UPlayerSkillComponent : public USkillComponent
{
	GENERATED_BODY()
	
public:
	UPlayerSkillComponent();

public:
	void SetWeapon(class AWeapon* _Weapon) { m_Weapon = _Weapon; }

public:
	void Bind(UEnhancedInputComponent* _EIC, class UInputContainer* _InputContainer);

	virtual void HitBoxOn() override;

protected:
	class AWeapon* m_Weapon;
};
