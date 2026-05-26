// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_HitBox.h"
#include "SkillComponent.h"


void UANS_HitBox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	pSkillCom->HitBoxOn();
}

void UANS_HitBox::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	pSkillCom->HitBoxCheck();
}

void UANS_HitBox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	pSkillCom->HitBoxOff();
}
