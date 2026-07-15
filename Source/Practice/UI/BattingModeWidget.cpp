// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/BattingModeWidget.h"
#include "../UI/UIManager.h"

#include "Components/Image.h"

#include "MyPlayer.h"

#include "PitchProjectile.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Boxcomponent.h"
#include "Components/CanvasPanelSlot.h"

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
	Super::NativeConstruct();

	FGeometry ZoneGeo = m_ZoneScale->GetCachedGeometry();

	FVector2D ZoneSize = ZoneGeo.GetLocalSize();

	// Aim 중심점을 시작점으로 저장
	m_CenterPos = ZoneSize * 0.5f;

	m_Elapsed = 0.f;
	m_Duration = 0.f;

	UCanvasPanelSlot* WarningSlot = Cast<UCanvasPanelSlot>(m_WarningImg->Slot);
	m_StartWarningSize = WarningSlot->GetSize();
}

void UBattingModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	m_Elapsed += InDeltaTime;

	if (m_Elapsed <= m_Duration)
	{
		const float Alpha = FMath::Clamp(m_Elapsed / m_Duration, 0.f, 1.f);

		UCanvasPanelSlot* WarningSlot = Cast<UCanvasPanelSlot>(m_WarningImg->Slot);
		UCanvasPanelSlot* AimSlot = Cast<UCanvasPanelSlot>(m_AimImg->Slot);

		if (WarningSlot && AimSlot)
		{
			FVector2D WarningSize = WarningSlot->GetSize();
			FVector2D AimSize = AimSlot->GetSize();

			FVector2D NewSize = FMath::Lerp(
				m_StartWarningSize,
				AimSize,
				Alpha);

			WarningSlot->SetSize(NewSize);
		}
	}
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

			// 뷰포트 기준 -1 ~ +1 정규화
			float NormalizedX = (_XPos - ViewX * 0.5f) / (ViewX * 0.5f);
			float NormalizedY = (_YPos - ViewY * 0.5f) / (ViewY * 0.5f);

			// Zone UI 크기의 절반만큼만 이동 (Zone 밖으로 못 나가게)
			FGeometry ZoneGeometry = m_ZoneScale->GetCachedGeometry();
			FVector2D ZoneLocalSize = ZoneGeometry.GetLocalSize();

			Transform.Translation.X = NormalizedX * (ZoneLocalSize.X * 0.5f);
			Transform.Translation.Y = NormalizedY * (ZoneLocalSize.Y * 0.5f);

			Transform.Translation += m_CenterPos;

			m_AimImg->SetRenderTransform(Transform);
		}
	}
}

void UBattingModeWidget::InitAimPos_Alt(float _XPos, float _YPos)
{	
	// 마우스 좌표를 Screen좌표의 정 중앙에 오게 한다.
	FGeometry Geometry = m_AimImg->GetCachedGeometry();

	// Aim Image의 경우, 자신이 HUD 상 존재하는 위치
	FWidgetTransform transform = m_AimImg->RenderTransform;
	transform.Translation = m_CenterPos;
	m_AimImg->SetRenderTransform(transform);
}

void UBattingModeWidget::InitWarningPos_Alt(FVector _WorldPosition)
{
	if (!m_WarningImg) return;

	AMyPlayer* Player = Cast<AMyPlayer>(GetOwningPlayerPawn());
	if (!Player) return;

	UBoxComponent* StrikeZone = Player->GetStrikeZone();
	if (!StrikeZone) return;

	//---------------------------------
	// World → StrikeZone Local
	//---------------------------------
	FTransform ZoneTransform =
		StrikeZone->GetComponentTransform();

	FVector LocalPos =
		ZoneTransform.InverseTransformPosition(_WorldPosition);

	FVector Extent =
		StrikeZone->GetScaledBoxExtent();

	//---------------------------------
	// Box 기준 Normalize (-1 ~ 1)
	//---------------------------------
	float NormalizedX =
		FMath::Clamp(LocalPos.Y / Extent.Y, -1.f, 1.f);

	float NormalizedY =
		FMath::Clamp(LocalPos.Z / Extent.Z, -1.f, 1.f);

	//---------------------------------
	// UI Zone 기준 변환
	//---------------------------------
	FGeometry ZoneGeometry =
		m_ZoneScale->GetCachedGeometry();

	FVector2D ZoneLocalSize =
		ZoneGeometry.GetLocalSize();

	FWidgetTransform Transform =
		m_WarningImg->GetRenderTransform();

	Transform.Translation.X =
		NormalizedX * (ZoneLocalSize.X * 0.5f);

	Transform.Translation.Y =
		-NormalizedY * (ZoneLocalSize.Y * 0.5f);

	Transform.Translation += m_CenterPos;

	m_WarningImg->SetRenderTransform(Transform);
	m_WarningImg->SetVisibility(ESlateVisibility::Visible);
}

void UBattingModeWidget::SetWarningDelegate(APitchProjectile* _Projectile, float _Duration)
{
	m_CurrentProjectile = _Projectile;

	_Projectile->m_InitWarningPos.AddDynamic(this, &UBattingModeWidget::InitWarningPos_Alt);

	m_Elapsed = 0.f;
	m_Duration = _Duration;

	UCanvasPanelSlot* WarningSlot = Cast<UCanvasPanelSlot>(m_WarningImg->Slot);
	WarningSlot->SetSize(m_StartWarningSize);
}

void UBattingModeWidget::ChangeProjectileAttitude()
{
	m_CurrentProjectile->ChangeAttitude();
	m_CurrentProjectile = nullptr;
}

void UBattingModeWidget::FailedParrying()
{
	// 대응되어 있는 투사체를 폭발 시키고, Deregister
	m_CurrentProjectile->Explode();
}

void UBattingModeWidget::Impact()
{
	// 1. Timing
	const float MAX_TIMING_ERROR = 0.15f;		// 15ms

	// 예외 상황 : 만약 너무 일찍 휘둘렀다면, 캐릭터의 스턴이라던가 필요할 듯?
	float timeDiff = m_Duration - m_Elapsed;
	if (timeDiff > MAX_TIMING_ERROR)
	{
		if (AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwningPlayerPawn()))
		{
			// Exit하면 연출이 부자연스럽다... 뭔가 방법이 필요함.
			pPlayer->ToggleInteraction(EInteractionType::RClick);
			return;
		}
	}

	float TimingScore = FMath::Clamp(1.f - FMath::Abs(timeDiff) / MAX_TIMING_ERROR, 0.f, 1.f);

	// 2. Position - AimPos와 벗어났는가 가 실패기준임.
	UCanvasPanelSlot* AimSlot = Cast<UCanvasPanelSlot>(m_AimImg->Slot);
	const float MAX_POS_ERROR = AimSlot->GetSize().X;

	float diffPos = (m_WarningImg->GetRenderTransform().Translation - m_AimImg->GetRenderTransform().Translation).Length();
	float PositionScore = FMath::Clamp(1.f - diffPos / MAX_POS_ERROR, 0.f, 1.f);


	// 3. Charge
	const float MAX_CHARGE_DURATION = 2.f;
	float ChargeScore = FMath::Clamp(m_ChargeElapsed / MAX_CHARGE_DURATION, 0.f, 1.f);

	// 최소 차징 보정 (0.5 ~ 1.2배)
	float ChargeMultiplier = FMath::Lerp(0.5f, 1.2f, ChargeScore);

	float FinalScore =
		TimingScore *
		PositionScore *
		ChargeMultiplier;

	UE_LOG(LogTemp, Warning, TEXT("Score TIMING : %f"), TimingScore);
	UE_LOG(LogTemp, Warning, TEXT("Score POS : %f"), PositionScore);
	UE_LOG(LogTemp, Warning, TEXT("Score CHARGE : %f"), ChargeScore);


	// FinalScore에 따라 행동을 결정하기.
			// 해당 Score가 0이고,  패링 실패, 투사체가 폭발해야 함
	if (FinalScore <= 0.f)
	{
		FailedParrying();
	}
	else
	{
		ChangeProjectileAttitude();
	}

	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwningPlayerPawn()))
	{
		// Exit하면 연출이 부자연스럽다... 뭔가 방법이 필요함.
		pPlayer->ExitBattingMode();
	}
}

