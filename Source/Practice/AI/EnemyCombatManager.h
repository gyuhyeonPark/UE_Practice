// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnemyCombatManager.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UEnemyCombatManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	bool TryAcquireAttackSlot(class ANPC* _NPC);

	void ReleaseAttackSlot(class ANPC* NPC);

	bool HasAttackSlot(class ANPC* NPC) const;

private:
	UPROPERTY()
	TArray<TObjectPtr<class ANPC>> m_AttackingNPCs;

	int32 m_MaxAttackers = 3;
};
