// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillComponent.h"

// Input
#include "InputContainer.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"

UPlayerSkillComponent::UPlayerSkillComponent()
{

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
			_EIC->BindAction(pAction, ETriggerEvent::Started, this, &USkillComponent::UseSkill, i);
	}
}

void UPlayerSkillComponent::HitBoxOn()
{
	// 히트 박스 위치를 무기의 소켓 위치로 설정한다.
	m_PrevHitBoxSockPos = m_SkeletalMeshCom->GetSocketLocation(TEXT("HitBoxSock"));
	m_HitBoxOn = true;
}
