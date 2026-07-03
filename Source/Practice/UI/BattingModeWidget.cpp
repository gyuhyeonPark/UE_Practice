// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/BattingModeWidget.h"
#include "../UI/UIManager.h"

#include "Components/Image.h"

#include "MyPlayer.h"

#include "PitchProjectile.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Boxcomponent.h"

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
}

void UBattingModeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime); // ← Super 호출 빠졌는지 확인
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

FVector3d UBattingModeWidget::GetImpactWorldPos()
{
	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwningPlayerPawn()))
	{
		if (APlayerController* PC = Cast<APlayerController>(pPlayer->GetController()))
		{
			int32 ViewX, ViewY;
			PC->GetViewportSize(ViewX, ViewY);

			FVector2D curAimPos = m_AimImg->RenderTransform.Translation;
			float ScreenX = ViewX * 0.5f + curAimPos.X;
			float ScreenY = ViewY * 0.5f + curAimPos.Y;

			FVector RayOrigin, RayDir;
			PC->DeprojectScreenPositionToWorld(ScreenX, ScreenY, RayOrigin, RayDir);

			// 플레이어 앞 타격 평면 정의
			FVector PlayerPos = pPlayer->GetActorLocation();
			FVector PlaneCenter = PlayerPos + pPlayer->GetActorForwardVector() * 100.f;
			FVector PlaneNormal = -pPlayer->GetActorForwardVector();

			// Ray와 평면 교차점 계산
			float T = FVector::DotProduct(PlaneCenter - RayOrigin, PlaneNormal)
				/ FVector::DotProduct(RayDir, PlaneNormal);

			FVector ImpactPos = RayOrigin + RayDir * T;
			return FVector3d(ImpactPos);
		}
	}

	return FVector3d::ZeroVector;
}

void UBattingModeWidget::SetWarningDelegate(APitchProjectile* _Projectile)
{
	_Projectile->m_InitWarningPos.AddDynamic(this, &UBattingModeWidget::InitWarningPos_Alt);
}
