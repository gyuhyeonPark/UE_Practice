// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Deco_HasAttackSlot.h"
#include "../AI/EnemyCombatManager.h"
#include "MonsterController.h"
#include "NPC.h"

bool UDeco_HasAttackSlot::CalculateRawConditionValue(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMem) const
{
    ANPC* NPC = Cast<ANPC>(
        _OwnCom.GetAIOwner()->GetPawn()
    );

    if (!NPC)
        return false;

    UEnemyCombatManager* Manager = NPC->GetGameInstance()->GetSubsystem<UEnemyCombatManager>();

    return Manager && Manager->HasAttackSlot(NPC);
}
