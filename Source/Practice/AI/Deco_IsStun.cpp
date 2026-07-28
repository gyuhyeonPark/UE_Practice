// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Deco_IsStun.h"
#include "MonsterController.h"
#include "Monster.h"

UDeco_IsStun::UDeco_IsStun()
{
}

bool UDeco_IsStun::CalculateRawConditionValue(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMem) const
{
	AMonsterController* pController = Cast<AMonsterController>(_OwnCom.GetAIOwner());
	if (pController == nullptr)
		return false;

	AMonster* pMonster = Cast<AMonster>(pController->GetPawn());
	if (pMonster == nullptr)
		return false;

	return pMonster->IsStun();
}
