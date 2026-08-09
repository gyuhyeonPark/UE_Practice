// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillComponent.h"
#include "../GlobalEnum.h"
#include "PlayerSkillComponent.generated.h"

USTRUCT(BlueprintType)
struct FJudgementEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EParryJudgementType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UNiagaraSystem> EffectData;	// 비동기 로드에 대해선 TSoftObjectPtr<>로 선언하기.
};

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

	void ParryingFunc(EParryJudgementType _ParryType);

	UAnimMontage* GetBattingMontage() { return m_BattingMontage; }

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
	TArray<FJudgementEffectInfo> m_ParryingEffects;		// 생성시킬 투사체의 UCLASS 정보를 가리킴

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PerpectParrySlowRatio")
	float m_SlowRatio;
};
