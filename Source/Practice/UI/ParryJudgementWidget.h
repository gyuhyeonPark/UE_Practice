// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../GlobalEnum.h"
#include "ParryJudgementWidget.generated.h"

/**
 * BattingWidget 클래스로부터 판정 결과를 받아
 * 그에 대응하는 Image를 MainHUD 상에 출력해주는 클래스.
 */

USTRUCT(BlueprintType)
struct FJudgementImageInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EParryJudgementType SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UTexture2D> ImageData;	// 비동기 로드에 대해선 TSoftObjectPtr<>로 선언하기.
};

UCLASS()
class PRACTICE_API UParryJudgementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void Play(EParryJudgementType _Type);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "JudgeImages")
	TArray<FJudgementImageInfo> m_ImageSlot;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ImageFadeInOutAnim;

	UPROPERTY(meta = (BindWidget))
	class UImage* JudgementImg;
};
