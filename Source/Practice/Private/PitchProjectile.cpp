// Fill out your copyright notice in the Description page of Project Settings.


#include "PitchProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MyPlayer.h"
#include "Components/Boxcomponent.h"
#include "Components/WidgetComponent.h"
#include "../UI/BallWarningWidget.h"
#include "../UI/BattingModeWidget.h"
#include "../UI/UIManager.h"
#include "../UI/MainHUD.h"

// Sets default values
APitchProjectile::APitchProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = m_BallMesh;
}

// Called when the game starts or when spawned
void APitchProjectile::BeginPlay()
{
	Super::BeginPlay();

	m_PrevLocation = GetActorLocation();


	APlayerController* PC = GetWorld()->GetFirstPlayerController();

/*	m_WarningWidget = CreateWidget<UBallWarningWidget>(PC, m_WidgetClass);

	if (m_WarningWidget && PC)
	{
		m_WarningWidget->AddToViewport();
		m_WarningWidget->SetWidgetPos(m_Destination);
	}*/

	if (PC == nullptr)
		return;

	if (AUIManager* pUIMgr = Cast<AUIManager>(PC->GetHUD()))
	{
		if (AMyPlayer* pPlayer = Cast<AMyPlayer>(m_Target))
		{
			if (UBattingModeWidget* pBModeWidget = Cast<UBattingModeWidget>(pPlayer->GetBattingModeWidget()->GetUserWidgetObject()))
				pBModeWidget->SetWarningDelegate(this);
		}
	}

	if (m_InitWarningPos.IsBound())
	{
		FVector TargetPos;
		TargetPos.X = m_StrikeZone->GetComponentLocation().X;
		TargetPos.Y = m_Destination.Y;
		TargetPos.Z = m_Destination.Z;

		m_InitWarningPos.Broadcast(TargetPos);
	}
}

// Called every frame
void APitchProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector dirVec = m_Destination - GetActorLocation();
	dirVec.Normalize();

	FVector nextPos = m_PrevLocation + dirVec * m_PitchData.Speed * DeltaTime;

	SetActorLocation(nextPos);

	m_PrevLocation = nextPos;

	FVector prevDirVec = m_Destination - m_PrevLocation;
	prevDirVec.Normalize();

	if (dirVec.Dot(prevDirVec) < 0.f)
	{
/*		m_WarningWidget->RemoveFromParent();
		m_WarningWidget = nullptr;*/
		Destroy();
	}
}

void APitchProjectile::InitProjectile(APawn* _User, APawn* _Target, USkillDataBase* _Skill)
{
	m_SkillUser = _User;
	m_Target = _Target;
	m_Skill = _Skill;

	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(_Target))
	{
		m_StrikeZone = pPlayer->GetStrikeZone();

		// 해당 스트라이크 존 내부 중 랜덤으로 목적지를 설정
		FVector pZoneCenterPos = m_StrikeZone->GetComponentLocation();
/*		FVector pZoneScale = m_StrikeZone->GetComponentScale();*/
		FVector pZoneScale = m_StrikeZone->GetScaledBoxExtent();

		float RandY = FMath::RandRange(pZoneCenterPos.Y - pZoneScale.Y, pZoneCenterPos.Y + pZoneScale.Y);
		float RandZ = FMath::RandRange(pZoneCenterPos.Z - pZoneScale.Z, pZoneCenterPos.Z + pZoneScale.Z);

		m_Destination = FVector(pZoneCenterPos.X, RandY, RandZ);
	}
}

