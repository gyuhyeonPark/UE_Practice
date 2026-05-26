// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/InvenWidget.h"
#include "../Item/ItemInst.h"
#include "../UI/ToolTipWidget.h"

#include "Components/ListView.h"

void UInvenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 인벤토리 위젯은 툴팁으로 사용할 객체를 하나 만들어준다.
	if (ToolTipClass)
		ToolTip = Cast<UToolTipWidget>(CreateWidget(this, ToolTipClass));
}

void UInvenWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInvenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInvenWidget::RefreshItem(TArray<class UItemInst*>& _InvenItems)
{
	// 리스트 뷰에 들어있던 항목 전부 제거
	ListView->ClearListItems();
	ListView->RegenerateAllEntries();

	// Inventory로 부터 전달받은 데이터들을 ListView의 개별 Entity로 추가한다.
	for (UItemInst* pItem : _InvenItems)
	{
		if (pItem != nullptr)
		{
			ListView->AddItem(pItem);
		}
	}
}
