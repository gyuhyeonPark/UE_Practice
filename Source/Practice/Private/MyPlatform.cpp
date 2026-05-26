// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlatform.h"

#include "MyPlayerState.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

AMyPlatform::AMyPlatform()
{
	GetStaticMeshComponent()->SetGenerateOverlapEvents(false);		// 오버랩 이벤트 생성
	GetStaticMeshComponent()->SetNotifyRigidBodyCollision(true);	// Hit(Block 처리)	
}

void AMyPlatform::BeginPlay()
{
	Super::BeginPlay();

	// delegate 등록
	GetStaticMeshComponent()->OnComponentHit.AddDynamic(this, &AMyPlatform::OnPlatformHit);
}

void AMyPlatform::OnPlatformHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& _Hit)
{	
	if (IsScored)
		return;

	ACharacter* pCharacter = Cast<ACharacter>(_OtherActor);

	bool isCharacterOn = false;
	if (GetStaticMeshComponent()->GetRelativeLocation().Z < pCharacter->GetCapsuleComponent()->GetRelativeLocation().Z - pCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
		isCharacterOn = true;

	if (pCharacter && isCharacterOn)
	{
		AMyPlayerState* pPlayerState = pCharacter->GetController()->GetPlayerState<AMyPlayerState>();

		if (pPlayerState)
		{
			pPlayerState->AddJumpScore(10);
			IsScored = true;

			UE_LOG(LogTemp, Warning, TEXT("Add Score : %d, CurrentScore : %d"), 10, pPlayerState->GetjumpScore());

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Current Score : %d"),
				pPlayerState->GetjumpScore()));
		}
	}

}


