// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Item/ItemInst.h"
#include "InventoryManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PRACTICE_API UInventoryManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UDataTable* _Table, FName _RowName);


	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const TArray<UItemInst*>& GetInventoryItems() { return m_Inven; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(UItemInst* _TargetItem);

public:
	// 인벤토리에 있는 아이템을 InventoryUI 쪽에 반영.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UpdateInventory();

private:
	UPROPERTY()
	TArray<UItemInst*> m_Inven;
};
