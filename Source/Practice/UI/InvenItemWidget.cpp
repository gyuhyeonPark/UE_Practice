// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/InvenItemWidget.h"
#include "../UI/InvenWidget.h"
#include "../UI/ToolTipWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"

#include "../Item/ItemInst.h"

void UInvenItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Background->ToolTipWidgetDelegate.BindDynamic(this, &UInvenItemWidget::GetToolTipWidget);
}

void UInvenItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UInvenItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UInvenItemWidget::NativeOnListItemObjectSet(UObject* _Data)
{
	UItemInst* pItem = Cast<UItemInst>(_Data);

	if (pItem == nullptr)
		return;

	CurItem = pItem;
	ItemName->SetText(pItem->GetItemName());
	ItemCount->SetText(FText::AsNumber(pItem->GetItemCount()));
	IconImage->SetBrushFromTexture(pItem->GetIconTexture());
}

UWidget* UInvenItemWidget::GetToolTipWidget()
{
	// 자신을 소유한 Inventory 위젯에 접근 후,
	// Inventory Widget이 갖고 있는 ToolTip 위젯에 접근.
	if (UInvenWidget* pInvenWidget = GetOwningListView()->GetTypedOuter<UInvenWidget>())
	{
		// 해당 ToolTip Widget에게 EntryItem(본인)이 알고 있는 아이템의 주솟값을 넘겨준다.
		UToolTipWidget* pToolTip = pInvenWidget->GetToolTipWidget();
		if (pToolTip)
			pToolTip->SetCurItem(CurItem);

		return pToolTip;
	}
	return nullptr;
}
