// Fill out your copyright notice in the Description page of Project Settings.


#include "../Anim/ANS_FootStep.h"
#include "../Asset/PhysicalMaterial_FootSound.h"
#include "Kismet/GameplayStatics.h"

void UANS_FootStep::PlayFootstep(AActor* _Owner, USkeletalMeshComponent* _MeshCom)
{
	// 발이 땅에 닿은 _Owner(Actor) 가 어떤 발자국 소리를 낼 지 지형의 물리재질을 알아내서, 해당 재질에 등록되어 있는 소리를 재생한다.

	if (_Owner == nullptr)
		return;

	FVector vSockPos = _MeshCom->GetSocketLocation(m_SockName);
	vSockPos.Z += 20.f;		// 발이 지면이랑 겹칠 수도 있으니...

	FVector vEndPos = vSockPos;
	vEndPos.Z -= 100.f;

	UWorld* pWorld = _Owner->GetWorld();

	FCollisionQueryParams Param = {};
	Param.AddIgnoredActor(_Owner);				// 본인은 충돌 검사 무시
	Param.bReturnPhysicalMaterial = true;		// 피지컬 머티리얼 감지

	// 충돌 검사
	FHitResult HitResult = {};
	bool bHit = pWorld->LineTraceSingleByChannel(HitResult, vSockPos, vEndPos, ECC_Visibility, Param);

	if (bHit)
	{
		if (UPhysicalMaterial_FootSound* pMtrl = Cast<UPhysicalMaterial_FootSound>(HitResult.PhysMaterial))
		{
			if (USoundBase* pSound = pMtrl->GetFootStepSound())
			{
				UGameplayStatics::PlaySoundAtLocation(pWorld, pSound, HitResult.Location);
			}
		}
	}
}

void UANS_FootStep::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	PlayFootstep(MeshComp->GetOwner(), MeshComp);
}
