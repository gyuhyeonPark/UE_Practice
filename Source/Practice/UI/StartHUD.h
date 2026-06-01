// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartHUD.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UStartHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void StartButtonHovered();
	UFUNCTION()
	void StartButtonUnHovered();

	UFUNCTION()
	void QuitButtonClicked();

	UFUNCTION()
	void QuitButtonHovered();
	UFUNCTION()
	void QuitButtonUnHovered();

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitBtn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StartBtnFadeInAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StartBtnFadeOutAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* QuitBtnFadeInAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* QuitBtnFadeOutAnim;
};
