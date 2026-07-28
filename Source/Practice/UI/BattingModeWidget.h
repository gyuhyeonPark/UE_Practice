// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattingModeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UBattingModeWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 생성 시 최초 호출
	virtual void NativeOnInitialized() override;

	// 활성화 시 호출
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
/*	void SynchronizeUITransform();*/

protected:
	UFUNCTION()
	void UpdateAimPos_Alt(float _XPos, float _YPos);

	UFUNCTION()
	void InitAimPos_Alt(float _XPos, float _YPos);

	UFUNCTION()
	void InitWarningPos_Alt(FVector _WorldPosition);

public:
	void SetWarningDelegate(class APitchProjectile* _Projectile, float _Duration);

	void SetChargeElapsed(float _Elapsed) { m_ChargeElapsed = _Elapsed; }

	void ChangeProjectileAttitude();
	void FailedParrying();
public:
	void Impact();

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* m_AimImg;

	UPROPERTY(meta = (BindWidget))
	class UImage* m_ZoneScale;

/*	UPROPERTY(meta = (BindWidget))
	class UImage* m_ZoneImg;*/

	FVector2D m_CenterPos;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
	float m_AimSpeed;

	UPROPERTY(meta = (BindWidget))
	class UImage* m_WarningImg;

protected:
	float m_Elapsed;
	float m_Duration;	// 투사체의 도착 예정 시간 정보

	FVector2D m_StartWarningSize;

	float m_ChargeElapsed;
	class APitchProjectile* m_CurrentProjectile;
};
