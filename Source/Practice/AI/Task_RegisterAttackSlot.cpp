// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Task_RegisterAttackSlot.h"
#include "../AI/EnemyCombatManager.h"
#include "MonsterController.h"
#include "NPC.h"

UTask_RegisterAttackSlot::UTask_RegisterAttackSlot()
{
    // Task를 1개로 돌려쓰는 구조가
    bCreateNodeInstance = false;

    // 매 프레임마다 TickTask를 호출받을 지 설정
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
