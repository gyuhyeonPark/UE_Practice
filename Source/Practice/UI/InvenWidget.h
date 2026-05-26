// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InvenWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UInvenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 생성 시 최초 호출
	virtual void NativeOnInitialized() override;

	// 활성화 시 호출
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	class UToolTipWidget* GetToolTipWidget() const { return ToolTip; }

	void RefreshItem(TArray<class UItemInst*>& _InvenItems);

protected:
	UPROPERTY(meta = (BindWidget))
	class UListView* ListView;

	// 생성시킬 툴팁 자료형
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ToolTip")
	TSubclassOf<class UToolTipWidget> ToolTipClass;

	// 생성시킨 툴팁 객체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ToolTip")
	class UToolTipWidget* ToolTip;
};
