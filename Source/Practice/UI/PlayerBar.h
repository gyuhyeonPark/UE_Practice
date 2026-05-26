// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerBar.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UPlayerBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 생성 시 최초 호출
	virtual void NativeOnInitialized() override;

	// 활성화 시 호출
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// 플레이어가 데미지를 입으면 호출받을 Delegate 함수
	// MyPlayer의 부모 클래스의 TakeDamage 함수의 시그니처
	UFUNCTION()
	void UpdateHPBar(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, AController* _InstigatorController, AActor* _InstigatorActor);

	UFUNCTION()
	void UpdateHPBar_Alt(float _CurHP, float _MaxHP);

public:
	class UProgressBar* GetHPProgressBar() { return m_HPBar; }

	void UpdatePlayerHP(float _Ratio);

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* m_HPBar;
};
