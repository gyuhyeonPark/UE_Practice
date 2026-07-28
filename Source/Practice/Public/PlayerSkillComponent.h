// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillComponent.h"
#include "PlayerSkillComponent.generated.h"

UCLASS()
class PRACTICE_API UPlayerSkillComponent : public USkillComponent
{
	GENERATED_BODY()
	
public:
	UPlayerSkillComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Bind(UEnhancedInputComponent* _EIC, class UInputContainer* _InputContainer);

	void SetSkillSlotBattingMode() { m_SelectedSkillSlot = &m_PlayerSkillSlots; }

public:
	void StartCharge(int32 _SlotIndex, bool _IsBMode);
	void ChargeTick();
	void SwingFunc();

	virtual void SetWeapon(class AWeapon* _Weapon) override;
	void SwitchWeaponHand(bool _IsRight = true);

	void ParryingFunc();
protected:
	void CheckAndUseSkill(int32 _SlotIdx, bool _IsBMode);

protected:
	// 스킬 장착 가능 슬롯 (Blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSkills", meta = (TitleProperty = "SlotType"))
	TArray<FSkillSlotInfo>	m_PlayerSkillSlots;

	bool m_IsChargeOn;

	float m_ChargeElapsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMontage")
	UAnimMontage* m_BattingMontage;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UNiagaraComponent* m_ParryingNiagaraCom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ParryingEffect")
	class UNiagaraSystem* m_ParryingEffect;		// 생성시킬 투사체의 UCLASS 정보를 가리킴

};
