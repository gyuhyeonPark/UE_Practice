// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/UIManager.h"
#include "../UI/MainHUD.h"
#include "../UI/InvenWidget.h"
#include "../UI/BattingModeWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

AUIManager::AUIManager()
{
	// 아래 코드는 에셋의 경로가 바뀔 때 마다 수정해야 하기 때문에 좋지 않다.
	/*ConstructorHelpers::FClassFinder<UUserWidget> Finder(TEXT("/Game/UI/WBP_MainHUD.WBP_MainHUD_C"));
	if (Finder.Succeeded())
	{
		MainHUDClass = Finder.Class;
	}*/
}

void AUIManager::BeginPlay()
{
	Super::BeginPlay();

	if (m_MainHUDClass != nullptr)
	{
		m_MainHUD = Cast<UMainHUD>(CreateWidget(GetOwningPlayerController(), m_MainHUDClass));

		if (m_MainHUD)
		{
			m_MainHUD->AddToViewport();

			m_MainHUD->GetInventoryWidget()->SetVisibility(ESlateVisibility::Collapsed);
		}

	}
}

void AUIManager::UpdatePlayerHP(float _Ratio)
{
	m_MainHUD->UpdatePlayerHP(_Ratio);
}

void AUIManager::ToggleInventory()
{
	if (!m_MainHUD)
		return;

	UInvenWidget* pInvenWidget = m_MainHUD->GetInventoryWidget();
	if (!pInvenWidget)
		return;

	// 인벤토리 위젯의 현재 상태 확인
	ESlateVisibility Visible = pInvenWidget->GetVisibility();

	// 보인다면 끈다.
	if (Visible == ESlateVisibility::Visible)
	{
		pInvenWidget->SetVisibility(ESlateVisibility::Collapsed);

		APlayerController* pPC = GetOwningPlayerController();

		pPC->bShowMouseCursor = false;

		FInputModeGameOnly Mode;
		
		pPC->SetInputMode(Mode);
	}
	// 꺼져있으면 킨다
	else
	{
		pInvenWidget->SetVisibility(ESlateVisibility::Visible);

		APlayerController* pPC = GetOwningPlayerController();

		pPC->bShowMouseCursor = true;

		FInputModeGameAndUI Mode;
		Mode.SetWidgetToFocus(pInvenWidget->TakeWidget());
		pPC->SetInputMode(Mode);
	}
}

/*void AUIManager::ToggleBattingUI()
{
	if (!m_MainHUD)
		return;

	UBattingModeWidget* pBModeWidget = m_MainHUD->GetBattingModeWidget();
	if (!pBModeWidget)
		return;

	ESlateVisibility Visible = pBModeWidget->GetVisibility();

	if (Visible == ESlateVisibility::Visible)
	{
		pBModeWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		pBModeWidget->SetVisibility(ESlateVisibility::Visible);
	}
}*/

/*void AUIManager::SetBattingUITransform(FVector WorldCenter, FVector BoxExtent)
{
	UBattingModeWidget* Widget = m_MainHUD->GetBattingModeWidget();
	if (!Widget || Widget->GetVisibility() == ESlateVisibility::Collapsed)
		return;

	APlayerController* PC = GetOwningPlayerController();
	if (!PC)
		return;

	UCanvasPanelSlot* Slot = Cast<UCanvasPanelSlot>(Widget->Slot);
	if (!Slot)
		return;

	UWidget* Parent = Cast<UWidget>(Widget->GetParent());
	if (!Parent)
		return;

	FGeometry ParentGeo = Parent->GetCachedGeometry();

	FVector2D ScreenCenter;
	if (!PC->ProjectWorldLocationToScreen(WorldCenter, ScreenCenter, false))
		return;

	FVector2D LocalCenter = ParentGeo.AbsoluteToLocal(ScreenCenter);

	FVector2D ScreenTL;
	FVector2D ScreenBR;

	if (!PC->ProjectWorldLocationToScreen(
		WorldCenter + FVector(0, -BoxExtent.Y, BoxExtent.Z),
		ScreenTL,
		false))
		return;

	if (!PC->ProjectWorldLocationToScreen(
		WorldCenter + FVector(0, BoxExtent.Y, -BoxExtent.Z),
		ScreenBR,
		false))
		return;

	FVector2D LocalTL = ParentGeo.AbsoluteToLocal(ScreenTL);
	FVector2D LocalBR = ParentGeo.AbsoluteToLocal(ScreenBR);

	FVector2D Size(
		FMath::Abs(LocalBR.X - LocalTL.X),
		FMath::Abs(LocalBR.Y - LocalTL.Y)
	);

	Slot->SetPosition(LocalCenter);
	Slot->SetSize(Size);
}*/
