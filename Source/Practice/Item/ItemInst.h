// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemInst.generated.h"

/**
 * 실제 월드 상에 배치되는 Item 클래스.
 */
UCLASS(Blueprintable, BlueprintType)
class PRACTICE_API UItemInst : public UObject
{
	GENERATED_BODY()

public:
	virtual void UseItem(AActor* _User);

public:
	FName GetRowName() const { return m_RowName; }

	UFUNCTION(BlueprintCallable)
	virtual UDataTable* GetDataTable() const { return nullptr; }

	virtual void SetItemInfo(UDataTable* _Table, FName _RowName) { m_RowName = _RowName; }

	UFUNCTION(BlueprintCallable)
	FText GetItemName();

	UFUNCTION(BlueprintCallable)
	FText GetItemDescription();

	UFUNCTION(BlueprintCallable)
	int32 GetItemCount() { return m_Count; }
	UFUNCTION(BlueprintCallable)
	UTexture2D* GetIconTexture();

	void AddCount(int32 _Amount) { m_Count += _Amount; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FName m_RowName;		// 데이터 테이블에서 어떤 행을 가리키는 지

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	int32 m_Count;			// 아이템 누적 개수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString m_Boost;			// 아이템 실행 효과
};
