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

protected:
	// 스킬 장착 가능 슬롯 (Blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSkills", meta = (TitleProperty = "SlotType"))
	TArray<FSkillSlotInfo>	m_PlayerSkillSlots;

	bool m_IsChargeOn;

	float m_ChargeElapsed;

	UAnimMontage* m_BattingMontage;
};
