// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/EnemyCombatManager.h"
#include "NPC.h"

bool UEnemyCombatManager::TryAcquireAttackSlot(ANPC* _NPC)
{
    if (!_NPC)
        return false;

    // �̹� ������ ������ �ִٸ� ����
    if (m_AttackingNPCs.Contains(_NPC))
        return true;

    // ������ �� á�ٸ� ����
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
