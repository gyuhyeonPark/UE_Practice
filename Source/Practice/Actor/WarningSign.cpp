// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actor/WarningSign.h"
#include "Components/DecalComponent.h"
#include "Components/Boxcomponent.h"
#include "Components/WidgetComponent.h"
#include "PaperSpriteComponent.h"

#include "MyPlayer.h"
#include "../Interaction/InteractionComponent.h"
#include "PitchProjectile.h"

// Sets default values
AWarningSign::AWarningSign()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	m_DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	m_DecalComponent->SetupAttachment(RootComponent);

/*	m_MouseSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MouseUISprite"));
	m_MouseSprite->SetupAttachment(RootComponent);*/

	m_InRangeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BattingModeRangeBox"));
	m_InRangeBox->SetupAttachment(RootComponent);

	m_InRangeBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_InRangeBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_InRangeBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void AWarningSign::BeginPlay()
{
	Super::BeginPlay();
	
	UMaterialInterface* Mat = m_DecalComponent->GetDecalMaterial();

	m_DecalComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	m_MID = UMaterialInstanceDynamic::Create(Mat, this);

	m_DecalComponent->SetDecalMaterial(m_MID);

	m_MID->SetScalarParameterValue(TEXT("Percent"), 0.f);

	m_InRangeBox->OnComponentBeginOverlap.AddDynamic(
		this,
		&AWarningSign::RangeBeginOverlap);

	m_InRangeBox->OnComponentEndOverlap.AddDynamic(
		this,
		&AWarningSign::RangeEndOverlap);
}

// Called every frame
void AWarningSign::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsPlayerInRange)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
	}
}

void AWarningSign::SetPercent(float _Elapsed, float _Duration)
{
	if (m_MID)
	{
		m_MID->SetScalarParameterValue(TEXT("Percent"), _Elapsed / _Duration * 100.f);
	}
}

void AWarningSign::RangeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(OtherActor))
	{
		pPlayer->GetInteractionComponent()->RegisterInteractable(this);
	}

	m_IsPlayerInRange = true;
}

void AWarningSign::RangeEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(OtherActor))
	{
		pPlayer->GetInteractionComponent()->DeregisterInteractable(this);
	}
	m_IsPlayerInRange = false;
}

bool AWarningSign::CanInteract() const
{
	return m_IsPlayerInRange;
}

EInteractionType AWarningSign::GetInteractionType() const
{
	return EInteractionType::RClick;
}

void AWarningSign::Interact(AMyPlayer* _Player)
{
	if (_Player->GetCombatMode() == ECombatMode::NORMAL)
	{
		_Player->SetInteractionUISprite(nullptr);

		// Player 회전 시키기.
		FVector MyLocation = _Player->GetActorLocation();
		FVector TargetLocation = m_OwnerProjectile->GetPitcher()->GetActorLocation();

		FRotator LookAtRot =
			(TargetLocation - MyLocation).Rotation();

		// 캐릭터가 기울어지지 않도록 Pitch, Roll 제거
		LookAtRot.Pitch = 0.f;
		LookAtRot.Roll = 0.f;

		_Player->SetActorRotation(LookAtRot);

		if (_Player->GetController())
		{
			_Player->GetController()->SetControlRotation(LookAtRot);
		}

		// 투사체 또한 BattingMode로 진입하기
		m_OwnerProjectile->EnterBattingMode();
	}
	else if (_Player->GetCombatMode() == ECombatMode::BATTING)
	{
		const auto& pSpriteMap = _Player->GetInteractionComponent()->GetUISpriteMap();

		if (UPaperSprite* Sprite =
			pSpriteMap.Find(GetInteractionType())->Get())
		{
			_Player->SetInteractionUISprite(Sprite);
		}

		// 투사체를 기존 위치에 떨어뜨리는 로직?
		m_OwnerProjectile->ExitBattingMode();
	}

	_Player->BattingModeToggle();
}

void AWarningSign::CancleInteract(AMyPlayer* Player)
{
	Player->ExitBattingMode();
}



