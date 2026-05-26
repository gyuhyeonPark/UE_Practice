// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/ItemInst.h"
#include "ItemInst_CS.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UItemInst_CS : public UItemInst
{
	GENERATED_BODY() 

public:
	virtual void UseItem(AActor* _User) override;
	virtual UDataTable* GetDataTable() const { return m_ItemTable; }
	virtual void SetItemInfo(UDataTable* _Table, FName _RowName) { m_RowName = _RowName; m_ItemTable = _Table; }

protected: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (RowStructure = "/Script/Practice.ItemData_Consumable"))
	UDataTable* m_ItemTable;

};
