// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GenericTeamAgentInterface.h"

// Sets default values
AProjectileBase::AProjectileBase()
	: m_ProjectileRadius(30.f), m_MaxHitCount(1), m_CurHitCount(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. 루트 컴포넌트
	m_Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_Sphere->InitSphereRadius(m_ProjectileRadius);
	m_Sphere->SetCollisionProfileName(TEXT("ProjectileProfile"));
	RootComponent = m_Sphere;

	// 2. PMC (Projectile Movement Component)
	m_PMC = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PMC"));
	m_PMC->InitialSpeed = 2000.f;
	m_PMC->MaxSpeed = 2000.f;
	m_PMC->bShouldBounce = true;
	m_PMC->Bounciness = 0.6f;

	// 3. NiagaraComponent
	m_NiagaraCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	m_NiagaraCom->SetupAttachment(m_Sphere);
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	// 나이아가라 재생
	if (m_NiagaraCom && m_ProjectileEffect)
	{
		m_NiagaraCom->SetAsset(m_ProjectileEffect);
	}

	SetLifeSpan(5.f);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_CurHitCount < m_MaxHitCount)
		DetectCollision();
}

void AProjectileBase::DetectCollision()
{
	FVector CurPos = GetActorLocation();

	FCollisionShape Sphere = FCollisionShape::MakeSphere(m_ProjectileRadius);

	TArray<FHitResult> HitResults;

	// 투사체 자신과, 자신을 발사한 사용자는 충돌 검사 무시
	FCollisionQueryParams Params = {};
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(m_SkillUser);

	bool bHit = GetWorld()->SweepMultiByChannel(HitResults
		, m_PrevPos
		, CurPos
		, FQuat::Identity
		, ECC_EngineTraceChannel4
		, Sphere
		, Params);

	if (bHit)
	{
		for (const FHitResult& result : HitResults)
		{
			// 충돌한 액터
			AActor* pHitActor = result.GetActor();

			// 중복충돌 검사
			if (pHitActor && !m_HitActors.Contains(pHitActor))
			{
				// 최초 충돌이면 액터를 기록하고 충돌관련 처리진행
				m_HitActors.Add(pHitActor);

				if (IsHit(pHitActor))
				{
					++m_CurHitCount;

					// 충돌 시 할 일
					OnHit(pHitActor, result);

					// 히트 이펙트 재생 요청
					if (m_HitEffect)
					{
						// 충돌 위치에, m_HitEffect 나이아가라 재생하는 Actor 생성
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld()
							, m_HitEffect
							, result.ImpactPoint
							, result.ImpactNormal.Rotation());
					}

					if (m_CurHitCount >= m_MaxHitCount)
						Destroy();
				}
			}
		}
	}

	m_PrevPos = CurPos;
}

// 히트 판정조건
bool AProjectileBase::IsHit_Implementation(AActor* _HitActor)
{
	if (!GetInstigator() || !_HitActor)
		return false;

	// 투사체 발사 액터의 팀 확인
	const IGenericTeamAgentInterface* pInstigator = Cast<IGenericTeamAgentInterface>(GetInstigator());
	if (pInstigator == nullptr)
		return false;

	if (!Cast<IGenericTeamAgentInterface>(_HitActor))
		return false;

	// 투사체에 맞은 액터와 투사체를 발사한 액터의 관계를 확인.
	ETeamAttitude::Type Attitude = pInstigator->GetTeamAttitudeTowards(*_HitActor);

	return Attitude == ETeamAttitude::Hostile;
}

void AProjectileBase::OnHit_Implementation(AActor* _HitActor, const FHitResult& _Result)
{
	// 데미지를 준다

}