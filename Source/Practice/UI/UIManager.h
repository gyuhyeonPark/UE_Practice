// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "UIManager.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API AUIManager : public AHUD
{
	GENERATED_BODY()
	
public:
	AUIManager();

public:
	virtual void BeginPlay() override;

public:
	class UMainHUD* GetMainHUD() { return m_MainHUD; }
	void UpdatePlayerHP(float _Ratio);

	void ToggleInventory();

/*	void ToggleBattingUI();*/
/*	void SetBattingUITransform(FVector _WorldLocation, FVector _WorldScale);*/

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> m_MainHUDClass;

	class UMainHUD* m_MainHUD;
};
