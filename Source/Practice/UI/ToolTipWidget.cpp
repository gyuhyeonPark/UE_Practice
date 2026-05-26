// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/ToolTipWidget.h"
#include "../Item/ItemInst.h"
#include "Components/TextBlock.h"

void UToolTipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CurItem)
		ItemDesc->SetText(CurItem->GetItemDescription());
}
