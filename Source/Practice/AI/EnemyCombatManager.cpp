// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/EnemyCombatManager.h"
#include "NPC.h"

bool UEnemyCombatManager::TryAcquireAttackSlot(ANPC* _NPC)
{
    if (!_NPC)
        return false;

    // 이미 슬롯을 가지고 있다면 성공
    if (m_AttackingNPCs.Contains(_NPC))
        return true;

    // 슬롯이 꽉 찼다면 실패
    if (m_AttackingNPCs.Num() >= m_MaxAttackers)
        return false;

    m_AttackingNPCs.Add(_NPC);

    return true;
}

void UEnemyCombatManager::ReleaseAttackSlot(ANPC* NPC)
{
    if (!NPC)
        return;

    m_AttackingNPCs.Remove(NPC);
}

bool UEnemyCombatManager::HasAttackSlot(ANPC* NPC) const
{
    int32 pFound = m_AttackingNPCs.Find(NPC);

    if (pFound == INDEX_NONE)
        return false;

    return true;
}
