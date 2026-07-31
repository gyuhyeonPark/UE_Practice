// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actor/Explosion.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

#include "GlobalData.h"

// Sets default values
AExplosion::AExplosion()
	: m_Damage(10.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_SphereCollision->InitSphereRadius(500.f);
	m_SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	RootComponent = m_SphereCollision;

	m_NiagaraCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	m_NiagaraCom->SetupAttachment(m_SphereCollision);

/*	m_NiagaraCom->OnSystemFinished.AddDynamic(
		this,
		&AExplosion::OnEffectFinished);*/
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();

	if (m_NiagaraCom && m_HitEffect)
	{
		m_NiagaraCom->SetAsset(m_HitEffect);
	}

	// 확정타가 아니라면, Collision으로 데미지 줄 예정.
	if (!m_IsForceDamage)
		m_SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AExplosion::OnSphereBeginOverlap);
	else
	{
		UGameplayStatics::ApplyDamage(m_Target,
			m_Damage,
			nullptr,
			nullptr,
			UExplosionDamageType::StaticClass());
	}
	UE_LOG(LogTemp, Warning,
		TEXT("CanEverTick=%d StartEnabled=%d TickEnabled=%d"),
		PrimaryActorTick.bCanEverTick,
		PrimaryActorTick.bStartWithTickEnabled,
		IsActorTickEnabled());
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Elapsed += DeltaTime;

	if (m_Elapsed >= 0.75f)
	{
		Destroy();
	}
}

void AExplosion::OnEffectFinished(UNiagaraComponent* FinishedComponent)
{
	Destroy();
}

void AExplosion::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (ACharacter* pTargetCharacter = Cast<ACharacter>(OtherActor))
		{
			if (pTargetCharacter == m_Target)
			{
				UGameplayStatics::ApplyDamage(OtherActor,
					m_Damage,
					nullptr,
					nullptr,
					UExplosionDamageType::StaticClass());
			}
		}
	}
}

