// Fill out your copyright notice in the Description page of Project Settings.


#include "../AI/Deco_IsInRange.h"
#include "MonsterController.h"
#include "Monster.h"
#include "SkillComponent.h"
#include "SkillDataBase.h"

#include "BehaviorTree/BlackboardComponent.h"


UDeco_IsInRange::UDeco_IsInRange()
{
}

bool UDeco_IsInRange::CalculateRawConditionValue(UBehaviorTreeComponent& _OwnCom, uint8* _NodeMem) const
{
	AMonsterController* pController = Cast<AMonsterController>(_OwnCom.GetAIOwner());
	if (pController == nullptr)
		return false;

	AMonster* pMonster = Cast<AMonster>(pController->GetPawn());
	if (pMonster == nullptr)
		return false;

	UBlackboardComponent* pBBCom = _OwnCom.GetBlackboardComponent();
	if (pBBCom == nullptr)
		return false;

	AActor* pTargetActor = Cast<AActor>(pBBCom->GetValueAsObject(m_Target.SelectedKeyName));
	if (pTargetActor == nullptr)
		return false;

	float Dist = FVector::Dist(pMonster->GetActorLocation(), pTargetActor->GetActorLocation());

	float recogDist = 0.f;
	// SkillComponent를 통해 인식 거리를 가져온다.
	if (pMonster->GetSkillComponent()->GetSkillData(ESkillSlot::Skill_1))
		recogDist = pMonster->GetSkillComponent()->GetSkillData(ESkillSlot::Skill_1)->Range;

	return (Dist < recogDist);
}
