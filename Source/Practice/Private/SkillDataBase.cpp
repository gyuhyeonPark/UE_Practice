// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataBase.h"
#include "SkillComponent.h"
#include "ProjectileBase.h"

bool USkillDataBase::CanUseSkill_Implementation(APawn* _SkillUser, class USkillComponent* _SkillCom)
{
	// 쿨타임 돌아왔는지 체크
	if (_SkillCom->IsSkillCoolTime(GetPrimaryAssetId(), CoolTime))
	{
		float RemainTime = _SkillCom->GetSkillRemainCoolTime(GetPrimaryAssetId(), CoolTime);
		UE_LOG(LogTemp, Warning, TEXT("Skill CoolTime, RemainTime : %f"), RemainTime);
		return false;
	}

	return true;
}

void USkillDataBase::OnExecuteSkill_Implementation(APawn* _SkillUser, class USkillComponent* _SkillCom)
{
	// 쿨타임 등록
	_SkillCom->AddSkillUseTime(GetPrimaryAssetId());
}

void USkillDataBase::OnEndSkill_Implementation(APawn* _SkillUser, class USkillComponent* _SkillCom)
{

}

void USkillDataBase::OnFire_Implementation(APawn* _SkillUser, USkillComponent* _SkillCom)
{
	if (ProjectileClass == nullptr || _SkillUser == nullptr)
		return;

	// 투사체를 발사시킬 위치 찾기
	FVector InitPos = _SkillUser->GetActorLocation();
	FRotator InitRot = _SkillUser->GetController()->GetControlRotation();

	// 스킬 사용자의 스켈레탈 메시 컴포넌트가 있고, FireSock이 존재한다면
	if (_SkillCom->GetMesh() && _SkillCom->GetMesh()->DoesSocketExist(TEXT("FireSock")))
	{
		FTransform trans = _SkillCom->GetMesh()->GetSocketTransform(TEXT("FireSock"));
		// FireSock 소켓이 있다면, 해당 소켓의 위치를 투사체 생성 위치로 설정한다.
		InitPos = trans.GetLocation();
	}

	// SkillComponent 에서 설정해둔 투사체를 생성한다.
	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnParam = {};

		// 투사체를 생성시키는 플레이어를 소유자로 설정
		SpawnParam.Owner = _SkillUser;

		// 투사체 유발자 (데미지 공식 등의 권한을 지닌)
		SpawnParam.Instigator = _SkillUser;

		// 투사체 생성, ProjectileClass(UCLASS 정보)를 토대로 실제 생성 시 원하는 자료형 정보 전달.
		AProjectileBase* pProjectile = _SkillUser->GetWorld()->
			SpawnActor<AProjectileBase>(
				ProjectileClass,
				InitPos,
				InitRot,
				SpawnParam);

		// 투사체에게 사용자와 스킬을 알려준다.
		pProjectile->InitProjectile(_SkillUser, this);
	}
}
