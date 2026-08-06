// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataPlayer.h"
#include "MyPlayer.h"
#include "PlayerSkillComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

bool USkillDataPlayer::CanUseSkill_Implementation(APawn* _SkillUser, USkillComponent* _SkillCom)
{
	if (!Super::CanUseSkill_Implementation(_SkillUser, _SkillCom))
		return false;


	return true;
}

void USkillDataPlayer::OnExecuteSkill_Implementation(APawn* _SkillUser, USkillComponent* _SkillCom)
{
	Super::OnExecuteSkill_Implementation(_SkillUser, _SkillCom);

	AMyPlayer* pPlayer = Cast<AMyPlayer>(_SkillUser);
	if (pPlayer == nullptr)
	{
		// 플레이어가 아닌 다른 캐릭터가 접근하고 있다.
		UE_LOG(LogTemp, Error, TEXT("SkillUser is not Player"));
		return;
	}

	// 몽타주 재생
	if (pPlayer->GetMesh()->GetAnimInstance() && Montage != nullptr)
		pPlayer->GetMesh()->GetAnimInstance()->Montage_Play(Montage, MontageSpeed);

	// 플레이어 상태 변경
	if (_SkillUser->HasAuthority() || _SkillUser->IsLocallyControlled()) 
	{
		if (CanMove)
		{
			pPlayer->ChangePlayerState(EPlayerState::UseMoveSkill);
			pPlayer->SetMoveScale(MoveSpeedScale);
		}
		else
		{
			pPlayer->ChangePlayerState(EPlayerState::UseSkill);
			pPlayer->SetMoveScale(0.f);
		}

		pPlayer->SetRotateScale(RotateSpeed);
	}
}

void USkillDataPlayer::OnEndSkill_Implementation(APawn* _SkillUser, USkillComponent* _SkillCom)
{
	AMyPlayer* pPlayer = Cast<AMyPlayer>(_SkillUser);

	// Stun으로 인한 EndSkill 호출일 경우
	if (!pPlayer->IsStun())
	{
		pPlayer->SetMoveScale(1.f);
		pPlayer->SetRotateScale(1.f);

		if (pPlayer->GetCombatMode() == ECombatMode::BATTING && pPlayer->IsParrying())
			pPlayer->ExitBattingMode();
	}
}
