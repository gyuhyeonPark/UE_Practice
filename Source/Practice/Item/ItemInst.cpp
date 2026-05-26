// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/ItemInst.h"
#include "GlobalData.h"

void UItemInst::UseItem(AActor* _User)
{
}

FText UItemInst::GetItemName()
{
	UDataTable* pTable = GetDataTable();

	if (pTable == nullptr)
		return FText();

	FItemData* pData = pTable->FindRow<FItemData>(m_RowName, TEXT(""));
	
	if (pData == nullptr)
		return FText();

	return pData->ItemName;
}

FText UItemInst::GetItemDescription()
{
	if (!GetDataTable())
		return FText();

	FItemData* pData = GetDataTable()->FindRow<FItemData>(m_RowName, TEXT(""));
	if (!pData)
		return FText();

	return pData->ItemDesc;
}

UTexture2D* UItemInst::GetIconTexture()
{
	UDataTable* pTable = GetDataTable();

	if (pTable == nullptr)
		return nullptr;

	FItemData* pData = pTable->FindRow<FItemData>(m_RowName, TEXT(""));
	if (pData == nullptr)
		return nullptr;

	return pData->IconTex;
}
