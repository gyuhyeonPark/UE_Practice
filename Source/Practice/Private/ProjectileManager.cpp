// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileManager.h"
#include "../Actor/WarningSign.h"
#include "Components/Boxcomponent.h"
#include "ProjectileManageData.h"

void UProjectileManager::RegisterProjectileBox()
{
	TSubclassOf<AWarningSign> WarningSignClass = m_Data->WarningSignClass;
	
	if (!WarningSignClass)
		return;

	AWarningSign* WarningSignCDO =
		WarningSignClass->GetDefaultObject<AWarningSign>();

	if (!WarningSignCDO)
		return;

	UBoxComponent* RangeBox = WarningSignCDO->GetRangeBox();

	if (!RangeBox)
		return;

	const FVector BoxExtent = RangeBox->GetUnscaledBoxExtent();

	// WarningSign Box Y → Projectile X
	// WarningSign Box Z → Projectile Y
	m_ProjectileRange.X = BoxExtent.Y;
	m_ProjectileRange.Y = BoxExtent.Z;
}

FVector UProjectileManager::SelectDestination(FVector _PlayerLocation)
{
	if (!m_WaveBox)
		return FVector::ZeroVector;

	const FVector WaveCenter = m_WaveBox->GetComponentLocation();
	const FVector WaveExtent = m_WaveBox->GetScaledBoxExtent();

	const int32 MaxTryCount = 100;

	for (int32 i = 0; i < MaxTryCount; ++i)
	{
		float RangeRadius = m_Data->RangeRadius;

		// 플레이어 주변 랜덤 위치
		const FVector2D RandomOffset =
			FMath::RandPointInCircle(RangeRadius);

		FVector Candidate;

		Candidate.X = _PlayerLocation.X + RandomOffset.X;
		Candidate.Y = _PlayerLocation.Y + RandomOffset.Y;
		Candidate.Z = _PlayerLocation.Z;

		// WaveBox 월드 좌표 기준 범위 검사
		const bool bInsideWaveBox =
			FMath::Abs(Candidate.X - WaveCenter.X) <= WaveExtent.X &&
			FMath::Abs(Candidate.Y - WaveCenter.Y) <= WaveExtent.Y;

		if (!bInsideWaveBox)
			continue;

		// 기존 투사체와 겹치는지 검사
		if (!IsValidDestination(Candidate))
			continue;

		// 위치 예약
		m_ProjectileLocations.Add(Candidate);

		return Candidate;
	}

	UE_LOG(LogTemp, Warning,
		TEXT("ProjectileManager::SelectDestination() - Failed to find valid destination."));

	return FVector::ZeroVector;
}

void UProjectileManager::UnregisterProjectileLocation(const FVector& _Location)
{
	int32 IsDeleted = m_ProjectileLocations.RemoveSingle(_Location);

	if (IsDeleted == 0)
		UE_LOG(LogTemp, Warning, TEXT("Failed To Manage ProjectilePool"));
}

bool UProjectileManager::IsValidDestination(const FVector& _Candidate)
{
	for (const FVector& ProjectileLocation : m_ProjectileLocations)
	{
		const float XDistance =
			FMath::Abs(_Candidate.X - ProjectileLocation.X);

		const float YDistance =
			FMath::Abs(_Candidate.Y - ProjectileLocation.Y);

		// Projectile RangeBox가 서로 겹치는지 확인
		if (XDistance < m_ProjectileRange.X * 2.f &&
			YDistance < m_ProjectileRange.Y * 2.f)
		{
			return false;
		}
	}

	return true;
}
