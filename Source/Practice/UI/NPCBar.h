// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCBar.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UNPCBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 생성 시 최초 호출
	virtual void NativeOnInitialized() override;

	// 활성화 시 호출
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
	void UpdateHPBar(float _CurHP, float _MaxHP);

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* m_HPBar;
};
