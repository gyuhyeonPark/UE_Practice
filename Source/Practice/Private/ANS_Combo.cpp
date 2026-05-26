// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_Combo.h"
#include "SkillComponent.h"

void UANS_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	pSkillCom->OpenComboWindow();
}

void UANS_Combo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	// notify 이후 매 프레임 호출되는 함수.
	// 충돌 감지 등을 처리하면 좋을 듯.
}

void UANS_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	USkillComponent* pSkillCom = MeshComp->GetOwner()->GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return;

	pSkillCom->CloseComboWindow();
}
