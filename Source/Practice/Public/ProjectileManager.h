#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ProjectileManager.generated.h"

UCLASS()
class PRACTICE_API UProjectileManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void RegisterWaveBox(class UBoxComponent* _WaveBox)
	{
		m_WaveBox = _WaveBox;
	}

	UFUNCTION(BlueprintCallable)
	void RegisterProjectileBox();

	// 플레이어 주변에서 투사체가 생성될 위치를 반환
	UFUNCTION()
	FVector SelectDestination(FVector _PlayerLocation);

	// 투사체가 사라졌을 때 해당 위치를 제거
	void UnregisterProjectileLocation(const FVector& _Location);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<class UProjectileManageData> m_Data;

protected:
	bool IsValidDestination(const FVector& _Candidate);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaveBox")
	class UBoxComponent* m_WaveBox;

	// 현재 사용 중인 투사체들의 위치
	UPROPERTY()
	TArray<FVector> m_ProjectileLocations;

	FVector2D m_ProjectileRange;
};