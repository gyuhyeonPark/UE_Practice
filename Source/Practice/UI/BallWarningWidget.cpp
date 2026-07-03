
// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/BallWarningWidget.h"
#include "Components/Image.h"

void UBallWarningWidget::NativeConstruct()
{

}

void UBallWarningWidget::SetWidgetPos(FVector _WorldDest)
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    FVector2D ScreenPosition;
    bool bSuccess = PC->ProjectWorldLocationToScreen(_WorldDest, ScreenPosition);

    if (bSuccess)
    {
        // 위젯 중앙 기준으로 보정
        FVector2D WidgetSize = GetDesiredSize();
        ScreenPosition -= WidgetSize * 0.5f;

        SetPositionInViewport(ScreenPosition, false);  // false = 절대 스크린 좌표
    }
    else
    {
        // 카메라 뒤에 있으면 숨김
        SetVisibility(ESlateVisibility::Hidden);
    }
}
