// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataMonster.h"
#include "Monster.h"

bool USkillDataMonster::CanUseSkill_Implementation(APawn* _SkillUser, USkillComponent* _SkillCom)
{
	if (!Super::CanUseSkill_Implementation(_SkillUser, _SkillCom))
		return false;

	return true;
}

void USkillDataMonster::OnExecuteSkill_Implementation(APawn* _SkillUser, USkillComponent* _SkillCom)
{
	Super::OnExecuteSkill_Implementation(_SkillUser, _SkillCom);

	AMonster* pMonster = Cast<AMonster>(_SkillUser);
	if (pMonster == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillUser is not Monster"));
		return;
	}

	// 몽타주 재생
	if (pMonster->GetMesh()->GetAnimInstance())
		pMonster->GetMesh()->GetAnimInstance()->Montage_Play(Montage, MontageSpeed);

	// 몬스터 상태 변경

}

void USkillDataMonster::OnEndSkill_Implementation(APawn* _SkillUser, USkillComponent* _SkillCom)
{
	AMonster* pMonster = Cast<AMonster>(_SkillUser);

}

