// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Combo.h"
#include "SkillComponent.h"

void UANS_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	APawn* pSkillUser = Cast<APawn>(MeshComp->GetOwner());

	if (pSkillUser->IsLocallyControlled())
	{
		pSkillCom->OpenComboWindow();
	}
}

void UANS_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	APawn* pSkillUser = Cast<APawn>(MeshComp->GetOwner());

	if (pSkillUser->IsLocallyControlled())
	{
		pSkillCom->CloseComboWindow();
	}
}
