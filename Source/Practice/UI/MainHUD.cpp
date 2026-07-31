// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/MainHUD.h"
#include "../UI/PlayerBar.h"
#include "../UI/InvenWidget.h"
#include "../UI/BattingModeWidget.h"
#include "../UI/ParryJudgementWidget.h"

void UMainHUD::NativeOnInitialized()
{
}

void UMainHUD::NativeConstruct()
{
}

void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UMainHUD::UpdatePlayerHP(float _Ratio)
{
	PlayerBarWidget->UpdatePlayerHP(_Ratio);
}

void UMainHUD::PlaySwingUIAnimation(EParryJudgementType _Type)
{
	ParryJudgementWidget->Play(_Type);
}
