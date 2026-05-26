// Fill out your copyright notice in the Description page of Project Settings.


#include "../Item/InventoryManager.h"
#include "GlobalData.h"
#include "../Item/ItemInst_CS.h"
#include "../UI/InvenWidget.h"
#include "../UI/UIManager.h"
#include "../UI/MainHUD.h"

bool UInventoryManager::AddItem(UDataTable* _Table, FName _RowName)
{
	if (!_Table || _RowName.IsNone())
		return false;

	// 데이터 테이블에서, 인벤토리에 추가할 아이템을 검색한다.
	FItemData* pData = _Table->FindRow<FItemData>(_RowName, TEXT(""));
	if (!pData)
		return false;


	// 데이터 테이블에서 찾은 아이템이 이미 인벤토리에 있는지 확인한다.
	for (UItemInst* pItem : m_Inven)
	{
		// 이미 같은 종류의 아이템이 인벤토리에 있었다면
		if (pItem->GetRowName() == _RowName && _Table == pItem->GetDataTable())
		{
			pItem->AddCount(1);
			UpdateInventory();
			return true;
		}
	}

	// 해당 아이템을 최초 입수한 상황, 실제 아이템 객체를 만들어서 Array 에 추가한다.
	// 데이터 테이블에서 찾은 아이템의 타입정보를 확인한다.
	UItemInst* pNewItem = nullptr;

	switch (pData->ItemType)
	{
	case EItemType::Consumable:
	{
		pNewItem = NewObject<UItemInst_CS>();
		pNewItem->SetItemInfo(_Table, _RowName);
	}
	break;
	case EItemType::Equipment:


		break;
	case EItemType::Quest:


		break;
	}

	// 컨테이너에 추가
	pNewItem->AddCount(1);
	m_Inven.Add(pNewItem);

	UpdateInventory();

	return true;
}

void UInventoryManager::UpdateInventory()
{
	// HUD에 접근하기
	// 1. World를 가져온다
	if (GetWorld() == nullptr)
		return;

	// 2. PlayerController를 가져온다.
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC == nullptr)
		return;

	if (AUIManager* pUIMgr = Cast<AUIManager>(PC->GetHUD()))
	{
		if (UMainHUD* pMainHUD = pUIMgr->GetMainHUD())
		{
			UInvenWidget* pInvenWidget = pMainHUD->GetInventoryWidget();
			
			pInvenWidget->RefreshItem(m_Inven);
		}
	}
}
