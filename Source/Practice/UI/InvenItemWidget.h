// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InvenItemWidget.generated.h"

/**
 * InvenWidget의 부모 클래스.
 * Inventory UI의 한 아이템에 대한 UI 클래스.
 */
UCLASS()
class PRACTICE_API UInvenItemWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// InvenWidget의 ListView->AddItem 시 호출 되는 함수.
	virtual void NativeOnListItemObjectSet(UObject* _Data) override;	// List View에 추가되기 위해 반드시 구현되어야 하는 함수.

public:
	UFUNCTION()
	UWidget* GetToolTipWidget();

protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Background;

	UPROPERTY(meta = (BindWidget))
	class UImage* IconImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UItemInst* CurItem;

};
