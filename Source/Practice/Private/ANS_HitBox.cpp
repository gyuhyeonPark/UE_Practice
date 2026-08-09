// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_HitBox.h"
#include "SkillComponent.h"


void UANS_HitBox::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	APawn* pSkillUser = Cast<APawn>(MeshComp->GetOwner());

	pSkillCom->HitBoxOn();


/*	if (pSkillUser->IsLocallyControlled())
	{
		pSkillCom->HitBoxOn();
	}*/
}

void UANS_HitBox::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	APawn* pSkillUser = Cast<APawn>(MeshComp->GetOwner());

	pSkillCom->HitBoxCheck();
}

void UANS_HitBox::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	APawn* pSkillUser = Cast<APawn>(MeshComp->GetOwner());
	pSkillCom->HitBoxOff();
}
