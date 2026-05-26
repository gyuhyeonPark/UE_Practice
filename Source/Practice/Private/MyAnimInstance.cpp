// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyPlayer.h"
#include "SkillComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMyAnimInstance::AnimNotify_OnLandingStart()
{
	m_Character->JumpLock();

	UE_LOG(LogTemp, Warning, TEXT("Laninding!!!"));
	UE_LOG(LogTemp, Warning, TEXT("Laninding!!!"));
}

void UMyAnimInstance::AnimNotify_SkillEnd()
{
	m_Character->GetSkillComponent()->EndSkill();
}

void UMyAnimInstance::AnimNotify_Fire()
{
	m_Character->GetSkillComponent()->Fire();
}

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	m_Character = Cast<AMyPlayer>(TryGetPawnOwner());

	if (m_Character)
	{
		// ABP 소유 액터의 CharacterMovementComponent의 주소를 미리 받아둔다.
		m_MovementComponent = m_Character->GetCharacterMovement();
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float _DT)
{
	Super::NativeUpdateAnimation(_DT);

	if (!m_Character || !m_MovementComponent)
		return;

	FVector velocity = m_Character->GetVelocity();

	// 액터의 속력 계산
	// .Length() : 벡터의 길이 값 (스칼라)
	m_GroundSpeed = velocity.Size2D();	// z 성분 무시한 속력 값.

	if (m_GroundSpeed > 10.f)
		m_MovementDir = CalculateDirection(velocity, m_Character->GetActorRotation());

	// 캐릭터가 공중 상태인지 체크한다.
	m_IsFall = m_MovementComponent->IsFalling();

	// 캐릭터의 수직속도 측정
	m_VerticalSpeed = m_MovementComponent->Velocity.Z;

	// 월드 시간 누적값 저장
	m_AccWorldTime = GetWorld()->GetTimeSeconds();
}
