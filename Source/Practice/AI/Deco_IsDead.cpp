// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Deco_IsDead.h"
#include "MonsterController.h"
#include "Monster.h"

bool UDeco_IsDead::CalculateRawConditionValue(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMem) const
{
	AMonsterController* pController = Cast<AMonsterController>(_OwnCom.GetAIOwner());
	if (pController == nullptr)
		return false;

	AMonster* pMonster = Cast<AMonster>(pController->GetPawn());
	if (pMonster == nullptr)
		return false;

	return pMonster->IsDead();
}
