#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../GlobalEnum.h"

#include "GlobalData.generated.h"

// 공통 스탯
USTRUCT(BlueprintType)
struct FStatData : public FTableRowBase // DataTable 로 노출되기위해서 꼭 상속받아야 하는 구조체
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float	MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float	MaxMP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float	MaxSP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float	Att;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float	Def;
};

// Player 전용
USTRUCT(BlueprintType)
struct FPlayerStatData : public FStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float	MaxUP;
};

// Monster 전용
USTRUCT(BlueprintType)
struct FMonsterStatData : public FStatData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float	DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float	DetectRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float	LoseDetectRange;
};


// Item Data
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item | Property")
	EItemType	ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
	FText	ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
	FText	ItemDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
	UTexture2D*	IconTex; 
};

USTRUCT(BlueprintType)
struct FItemData_Consumable : public FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
	float HealAmount_HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
	float HealAmount_MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
	float HealAmount_SP;

public:
	FItemData_Consumable()
		: HealAmount_HP(0.f), HealAmount_MP(0.f), HealAmount_SP(0.f)
	{
		ItemType = EItemType::Consumable;
	}
};

USTRUCT(BlueprintType)
struct FItemData_Equipment : public FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
	EEquipType EquipType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
	float Att;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Property")
	float Def;

	// 무기 Mesh 등 추가 가능
public:
	FItemData_Equipment()
		: EquipType(EEquipType::Accessory), Att(0.f), Def(0.f)
	{
		ItemType = EItemType::Equipment;
	}
};