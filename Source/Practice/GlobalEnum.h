#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	Player UMETA(DisplayName = "Player"),
	Enemy UMETA(DisplayName = "Enemy"),
	None UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Consumable UMETA(DisplayName = "Consumable"),
	Equipment UMETA(DisplayName = "Equipment"),
	Quest UMETA(DisplayName = "Quest"),
};

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Accessory UMETA(DisplayName = "Accessory"),
};