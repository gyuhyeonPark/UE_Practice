// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/MainHUD.h"
#include "../UI/PlayerBar.h"
#include "../UI/InvenWidget.h"
#include "../UI/BattingModeWidget.h"
#include "../UI/ParryJudgementWidget.h"
#include "../UI/LockOnWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UMainHUD::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}

void UMainHUD::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMainHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (!LockOnWidget || !LockOnTarget.IsValid())
        return;

    APlayerController* PC = GetOwningPlayer();
    if (!PC)
        return;

    FVector2D ScreenPos;

    if (PC->ProjectWorldLocationToScreen(
        LockOnTarget->GetActorLocation(),
        ScreenPos))
    {
        FGeometry CanvasGeometry =
            RootCanvas->GetCachedGeometry();

        FVector2D LocalPos =
            CanvasGeometry.AbsoluteToLocal(
                CanvasGeometry.LocalToAbsolute(ScreenPos)
            );
        float ViewportScale =
            UWidgetLayoutLibrary::GetViewportScale(GetWorld());

        LocalPos /= ViewportScale;
        if (UCanvasPanelSlot* pSlot =
            Cast<UCanvasPanelSlot>(LockOnWidget->Slot))
        {
            pSlot->SetAlignment(FVector2D(0.5f, 0.5f));
            pSlot->SetPosition(LocalPos);
        }
    }
}

void UMainHUD::UpdatePlayerHP(float _Ratio)
{
	PlayerBarWidget->UpdatePlayerHP(_Ratio);
}

void UMainHUD::PlaySwingUIAnimation(EParryJudgementType _Type)
{
	ParryJudgementWidget->Play(_Type);
}

void UMainHUD::PlayLockOnUIAnimation(AActor* _TargetUnit)
{
    bLockOnActive = true;
    LockOnTarget = _TargetUnit;

    if (LockOnWidget)
    {
        LockOnWidget->SetVisibility(ESlateVisibility::Visible);
        LockOnWidget->Play();
    }
}

void UMainHUD::HideLockOnUI()
{
    bLockOnActive = false;

    if (LockOnWidget)
    {
        LockOnWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}
