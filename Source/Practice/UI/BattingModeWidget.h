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
	UFUNCTION()
	void UpdateAimPos_Alt(float _XPos, float _YPos);

	UFUNCTION()
	void InitAimPos_Alt(float _XPos, float _YPos);
	
	// 타격 시, 2차원 상의 UI 타격 Vector2D를 월드 위치로 변환하여 반환해주는 함수. 
	FVector3d GetImpactWorldPos();

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* m_AimImg;

	UPROPERTY(meta = (BindWidget))
	class UImage* m_ZoneScale;

	FVector2D m_AimingStartPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
	float m_AimSpeed;
};
