// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// 생성 시 최초 호출
	virtual void NativeOnInitialized() override;

	// 활성화 시 호출
	virtual void NativeConstruct() override;

	// 매 프레임 호출
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	class UPlayerBar* GetPlayerBar() { return PlayerBarWidget; }
	void UpdatePlayerHP(float _Ratio);

	class UInvenWidget* GetInventoryWidget() const { return InvenWidget; }

protected:
	UPROPERTY(meta = (BindWidget))
	class UPlayerBar* PlayerBarWidget;

	UPROPERTY(meta = (BindWidget))
	class UInvenWidget* InvenWidget;
};
