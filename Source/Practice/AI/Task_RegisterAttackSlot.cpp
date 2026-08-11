// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Task_RegisterAttackSlot.h"
#include "../AI/EnemyCombatManager.h"
#include "MonsterController.h"
#include "NPC.h"

UTask_RegisterAttackSlot::UTask_RegisterAttackSlot()
{
    // Task�� 1���� �������� ������
    bCreateNodeInstance = false;

    // �� �����Ӹ��� TickTask�� ȣ����� �� ����
    bNotifyTick = false;
}

EBTNodeResult::Type UTask_RegisterAttackSlot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ANPC* NPC = Cast<ANPC>(OwnerComp.GetAIOwner()->GetPawn());

    if (!NPC)
        return EBTNodeResult::Failed;

    UEnemyCombatManager* Manager = NPC->GetGameInstance()->GetSubsystem<UEnemyCombatManager>();

    if (!Manager)
        return EBTNodeResult::Failed;

    EBTNodeResult::Type pSuccessed = Manager->TryAcquireAttackSlot(NPC) ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

    return pSuccessed;
}
