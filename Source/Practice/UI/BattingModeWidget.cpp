// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/BattingModeWidget.h"

#include "Components/Image.h"

#include "MyPlayer.h"

void UBattingModeWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Player Actor의 OnTakeAnyDamage 델리게이트에 특정 멤버함수를 미리 등록해둔다.
	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwningPlayerPawn()))
	{
		pPlayer->m_UpdateAimPos.AddDynamic(this, &UBattingModeWidget::UpdateAimPos_Alt);
		pPlayer->m_InitAimPos.AddDynamic(this, &UBattingModeWidget::InitAimPos_Alt);
	}
}

void UBattingModeWidget::NativeConstruct()
{ 
	m_AimingStartPos = m_AimImg->RenderTransform.Translation;
}

void UBattingModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{

}

void UBattingModeWidget::UpdateAimPos_Alt(float _XPos, float _YPos)
{
	FWidgetTransform Transform = m_AimImg->GetRenderTransform();

	int32 ViewX, ViewY;
	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwningPlayerPawn()))
	{
		if (APlayerController* PC = Cast<APlayerController>(pPlayer->GetController()))
		{
			PC->GetViewportSize(ViewX, ViewY);

			float NormalizedX = (_XPos - ViewX * 0.5f) / (ViewX * 0.5f);
			float NormalizedY = (_YPos - ViewY * 0.5f) / (ViewY * 0.5f);

			FGeometry ZoneGeometry = m_ZoneImg->GetCachedGeometry();
			FVector2D ZoneUIScale = ZoneGeometry.GetAbsoluteSize();

			Transform.Translation.X = NormalizedX * (ZoneUIScale.X);
			Transform.Translation.Y = NormalizedY * (ZoneUIScale.Y) - 27.f;

			m_AimImg->SetRenderTransform(Transform);

		}
	}
}

void UBattingModeWidget::InitAimPos_Alt(float _XPos, float _YPos)
{	
	// 마우스 좌표를 Screen좌표의 정 중앙에 오게 한다.
	FGeometry Geometry = m_AimImg->GetCachedGeometry();
	FVector2D CenterPos =
		Geometry.GetAbsolutePositionAtCoordinates(
			FVector2D(0.5f, 0.5f));

	// UI 좌표 초기화.
	m_AimingStartPos = CenterPos;

	// Aim Image의 경우, 자신이 HUD 상 존재하는 위치
	FWidgetTransform transform = m_AimImg->RenderTransform;
	transform.Translation.X = 0.f;
	transform.Translation.Y = -27.f;
	m_AimImg->SetRenderTransform(transform);
	
}
