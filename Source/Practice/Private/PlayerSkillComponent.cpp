// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillComponent.h"

// Input
#include "InputContainer.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "SkillDataBase.h"

UPlayerSkillComponent::UPlayerSkillComponent()
{
	for (int32 i = (int32)ESkillSlot::COMMON_SECTION_END + 1; i < (int32)ESkillSlot::PLAYER_SECTION_END; ++i)
	{
		m_PlayerSkillSlots.Add(FSkillSlotInfo{ (ESkillSlot)i, });
	}
}

void UPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// 설정된 BattingMode 스킬의 Montage를 멤버에 Binding.
	m_BattingMontage = m_PlayerSkillSlots[0].SkillData->Montage;
}

void UPlayerSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_IsChargeOn)
		m_ChargeElapsed += DeltaTime;
	
}

void UPlayerSkillComponent::Bind(UEnhancedInputComponent* _EIC, UInputContainer* _InputContainer)
{
	if (_EIC == nullptr || _InputContainer == nullptr)
		return;

	// 일반 스킬 바인딩
	for (int32 i = 0; i < m_SkillSlots.Num(); ++i)
	{
		// InputAction 찾기
		FString SlotName = StaticEnum<ESkillSlot>()->GetNameStringByValue(i);

		FString IAName = FString::Printf(TEXT("IA_%s"), *SlotName);

		if (const UInputAction* pAction = _InputContainer->FindIAByName(IAName))
			_EIC->BindAction(pAction, ETriggerEvent::Started, this, &USkillComponent::UseSkill, i, false);
	}

	// Batting Mode Skills Binding
	if (const UInputAction* pAction = _InputContainer->FindIAByName(TEXT("IA_BLeftClick")))
	{
		_EIC->BindAction(pAction, ETriggerEvent::Started, this, &UPlayerSkillComponent::StartCharge, (int32)ESkillSlot::BATTING_SKILL, true);	// Skill 발동
		_EIC->BindAction(pAction, ETriggerEvent::Triggered, this, &UPlayerSkillComponent::ChargeTick);		// IA Trigger : 눌림
		_EIC->BindAction(pAction, ETriggerEvent::Completed, this, &UPlayerSkillComponent::SwingFunc);
	}
	
}

void UPlayerSkillComponent::StartCharge(int32 _SlotIndex, bool _IsBMode)
{
	// UseSkill에서는 각종 스킬 상태 관리 + Montage 재생이 될 예정
	UseSkill(_SlotIndex, _IsBMode);

	// AnimNotify로 인해 Montage_Pause 호출될 예정.

	m_IsChargeOn = true;
	m_ChargeElapsed = 0.f;
}

void UPlayerSkillComponent::ChargeTick()
{
}

void UPlayerSkillComponent::SwingFunc()
{
	// Montage 재개.
	m_SkeletalMeshCom->GetAnimInstance()->Montage_Resume(m_BattingMontage);
	m_SkeletalMeshCom->GetAnimInstance()->Montage_SetPlayRate(m_BattingMontage, 1.7f);
}
