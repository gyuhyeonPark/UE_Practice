// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actor/Explosion.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	m_SphereCollision->InitSphereRadius(10.f);
	m_SphereCollision->SetCollisionProfileName(TEXT("ProjectileProfile"));
	RootComponent = m_SphereCollision;

	m_NiagaraCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	m_NiagaraCom->SetupAttachment(m_SphereCollision);

	m_NiagaraCom->OnSystemFinished.AddDynamic(
		this,
		&AExplosion::OnEffectFinished);
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_NiagaraCom && m_HitEffect)
	{
		m_NiagaraCom->SetAsset(m_HitEffect);
	}
}

// Called every frame
void AExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosion::OnEffectFinished(UNiagaraComponent* FinishedComponent)
{
	Destroy();
}

