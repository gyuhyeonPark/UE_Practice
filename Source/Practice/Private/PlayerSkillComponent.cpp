// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillComponent.h"

// Input
#include "InputContainer.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "SkillDataBase.h"

#include "MyPlayer.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "../Item/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"

UPlayerSkillComponent::UPlayerSkillComponent()
	: m_SlowRatio(0.5f)
{
	for (int32 i = (int32)ESkillSlot::COMMON_SECTION_END + 1; i < (int32)ESkillSlot::PLAYER_SECTION_END; ++i)
	{
		m_PlayerSkillSlots.Add(FSkillSlotInfo{ (ESkillSlot)i, });
		m_ParryingEffects.Add(FJudgementEffectInfo{ (EParryJudgementType)i, });
	}

	m_ParryingNiagaraCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParryingImpact"));
	m_ParryingNiagaraCom->SetAutoActivate(false);   // 시작 시 자동 재생 X
	m_ParryingNiagaraCom->SetAllowScalability(true);
	m_ParryingNiagaraCom->SetRelativeScale3D(FVector(0.25f));
}

void UPlayerSkillComponent::BeginPlay()
{
	Super::BeginPlay();
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
		{
			_EIC->BindAction(pAction, ETriggerEvent::Started, this, &UPlayerSkillComponent::CheckAndUseSkill, i, false);
		}
	}

	// Batting Mode Skills Binding
	if (const UInputAction* pAction = _InputContainer->FindIAByName(TEXT("IA_BLeftClick")))
	{
		_EIC->BindAction(pAction, ETriggerEvent::Started, this, &UPlayerSkillComponent::StartCharge, (int32)ESkillSlot::BATTING_SKILL, true);	// Skill 발동
		_EIC->BindAction(pAction, ETriggerEvent::Triggered, this, &UPlayerSkillComponent::ChargeTick);		// IA Trigger : 눌림
		_EIC->BindAction(pAction, ETriggerEvent::Completed, this, &UPlayerSkillComponent::SwingFunc);
		_EIC->BindAction(pAction,
			ETriggerEvent::Canceled,
			this,
			&UPlayerSkillComponent::SwingFunc);
	}
	
}

void UPlayerSkillComponent::StartCharge(int32 _SlotIndex, bool _IsBMode)
{
	AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwner());
	if (pPlayer->IsStun())
		return;

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
	UE_LOG(LogTemp, Warning, TEXT("Swing Call, Charge Elapsed : %f"), m_ChargeElapsed);

	// 맘에 들진 않지만... 우선 BattingModeWidget에 m_ChargeElapsed를 어떻게든 전달해보자꾸나...
	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwner()))
	{
		pPlayer->SendChargeElapsed(m_ChargeElapsed);
	}

	m_IsChargeOn = false;
	m_ChargeElapsed = 0.f;

	// Montage 재개.
	if (m_BattingMontage)
	{
		if (m_SkeletalMeshCom->GetAnimInstance()->Montage_IsPlaying(m_BattingMontage))
		{
			m_SkeletalMeshCom->GetAnimInstance()->Montage_JumpToSection(TEXT("HitStart"));
		}
		else
			m_SkeletalMeshCom->GetAnimInstance()->Montage_Resume(m_BattingMontage);
		
		m_SkeletalMeshCom->GetAnimInstance()->Montage_SetPlayRate(m_BattingMontage, 1.7f);
	}
}

void UPlayerSkillComponent::ParryingFunc(EParryJudgementType _ParryType)
{
	if (_ParryType == EParryJudgementType::SICK)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), m_SlowRatio);
	}

	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwner()))
	{
		pPlayer->SwitchWeaponHand(false);
	}

	if (m_ParryingNiagaraCom && m_ParryingEffects[(int32)_ParryType].EffectData != nullptr)
	{
		m_ParryingNiagaraCom->DeactivateImmediate();
		m_ParryingNiagaraCom->SetAsset(m_ParryingEffects[(int32)_ParryType].EffectData.Get());
		m_ParryingNiagaraCom->Activate(true);

		m_ParryingNiagaraCom->SetWorldLocation(m_Weapon->GetHitSockPos());
		m_ParryingNiagaraCom->ActivateSystem();
	}
}

void UPlayerSkillComponent::CheckAndUseSkill(int32 _SlotIdx, bool _IsBMode)
{
	AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwner());

	if (pPlayer->IsStun())
		return;

	UseSkill(_SlotIdx, _IsBMode);
}
