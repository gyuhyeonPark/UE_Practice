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

#include "../Actor/WarningSign.h"
#include "../Actor/Explosion.h"

#include "GlobalData.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
APitchProjectile::APitchProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	m_BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = m_BallMesh;

	m_TrailNiagaraCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailImpact"));
	m_TrailNiagaraCom->SetAutoActivate(true);  
	m_TrailNiagaraCom->SetAllowScalability(true);
	m_TrailNiagaraCom->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APitchProjectile::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC == nullptr)
		return;

	// 1. WarningSign 생성 및 m_Destination 설정
	FTransform Trans;
	Trans.SetLocation(m_WarningSignLocation);
	m_WarningSign = m_SkillUser->GetWorld()->SpawnActor<AWarningSign>(
		WarningSigneClass,
		Trans,
		FActorSpawnParameters{}
	);

	m_WarningSign->RegisterProjectile(this);

	m_Duration = m_PitchData.Duration;

	InitBeforeShoot();

	if (m_TrailNiagaraCom && m_HostileTrailEffect)
	{
		m_TrailNiagaraCom->SetRelativeLocation(FVector::ZeroVector);
		m_TrailNiagaraCom->SetAsset(m_HostileTrailEffect.Get());
		m_TrailNiagaraCom->ReinitializeSystem();
	}
}

// Called every frame
void APitchProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Elapsed += DeltaTime;

	if (m_WarningSign)
		m_WarningSign->SetPercent(m_Elapsed, m_Duration);

	// BattingMode인 경우, 폭발에 대한 책임을 지지 않는다.
	if (m_Elapsed >= m_Duration && !m_IsBattingMode)
	{
		if (AMyPlayer* pTargetPlayer = Cast<AMyPlayer>(m_Target))
		{
			if (m_WarningSign)
				m_WarningSign->CancleInteract(pTargetPlayer);		// 상호작용 중일 때 Cancle.
		}
		Explode();

		return;
	}
	
	FVector nextPos = m_StartLocation + m_DirVec * m_Elapsed / m_Duration * m_Distance;

	SetActorLocation(nextPos);
}

void APitchProjectile::InitProjectile(APawn* _User, APawn* _Target, USkillDataBase* _Skill)
{
	m_SkillUser = _User;
	m_Target = _Target;
	m_Skill = _Skill;

	m_Destination = m_WarningSignLocation = m_Target->GetActorLocation();

	// Trail Niagara 설정
	if (m_TrailNiagaraCom)
	{
		if (Cast<AMyPlayer>(m_SkillUser) && m_FriendlyTrailEffect)
		{
			m_TrailNiagaraCom->SetAsset(m_FriendlyTrailEffect.Get());
			m_TrailNiagaraCom->ReinitializeSystem();
		}
		else if (m_HostileTrailEffect)
		{
			m_TrailNiagaraCom->SetAsset(m_HostileTrailEffect.Get());
			m_TrailNiagaraCom->ReinitializeSystem();
		}
	}
}

void APitchProjectile::EnterBattingMode()
{
	// 목적지를 재설정하되, 남은 시간은 동일해야 한다.
	// 아이디어 : m_StartLocation을 현재 위치로 설정.
	// m_Duration을 m_Duration - m_Elapsed로 설정.
	// m_Elapsed를 0.f로 설정.

	m_IsBattingMode = true;
	m_Duration -= m_Elapsed;

	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(m_Target))
	{
		if (UBattingModeWidget* pBModeWidget = Cast<UBattingModeWidget>(pPlayer->GetBattingModeWidget()->GetUserWidgetObject()))
		{
			// 1. 도착 지점 설정
			m_StrikeZone = pPlayer->GetStrikeZone();

			// 해당 스트라이크 존 내부 중 랜덤으로 목적지를 설정
			FVector pZoneCenterPos = m_StrikeZone->GetComponentLocation();
			FVector pZoneScale = m_StrikeZone->GetScaledBoxExtent();

			float RandY = FMath::RandRange(pZoneCenterPos.Y - pZoneScale.Y, pZoneCenterPos.Y + pZoneScale.Y);
			float RandZ = FMath::RandRange(pZoneCenterPos.Z - pZoneScale.Z, pZoneCenterPos.Z + pZoneScale.Z);

			m_Destination = FVector(pZoneCenterPos.X, RandY, RandZ);

			InitBeforeShoot();

			// 2. 도착 지점 표시 Delegate 등록
			pBModeWidget->SetWarningDelegate(this, m_Duration);

			// 3. 도착 시간 정보를 BattingMode에게 전달.

		}
	}

	GetWorld()->GetTimerManager().SetTimerForNextTick(
		this,
		&APitchProjectile::BroadCastWarningPosFunc);

	// Warning Sign은 안보이게 처리.
	if (m_WarningSign)
		m_WarningSign->SetActorHiddenInGame(true);
}

void APitchProjectile::ExitBattingMode()
{
	m_IsBattingMode = false;

	m_Duration -= m_Elapsed;

	m_Destination = m_WarningSignLocation;

	InitBeforeShoot();

	// Warning Sign은 다시 보이게 처리.
	if (m_WarningSign)
		m_WarningSign->SetActorHiddenInGame(false);
}

void APitchProjectile::BroadCastWarningPosFunc()
{
	// 2. 설정된 m_Destination 기반 투사체 관련 정보 설정
	if (m_InitWarningPos.IsBound())
	{
		FVector TargetPos;
		TargetPos.X = m_StrikeZone->GetComponentLocation().X;
		TargetPos.Y = m_Destination.Y;
		TargetPos.Z = m_Destination.Z;

		m_InitWarningPos.Broadcast(TargetPos);
	}
}

void APitchProjectile::ChangeAttitude(float _Speed)
{
	// 패링되었을 때의 로직
	// 플레이어 - 적군 1대1 대응 상정
	m_IsBattingMode = false;

	m_WarningSign->Destroy();
	m_WarningSign = nullptr;

	// SkillUser와 Target 반전
	InitProjectile(m_Target, m_SkillUser, nullptr);
	
	// 멤버 초기화
	InitBeforeShoot();

	// 속도를 통해 m_Duration을 갱신해준다.
	m_Duration = m_Distance / _Speed;
}

void APitchProjectile::Explode(bool _ForceDamage)
{
	FVector SpawnPos = GetActorLocation();

	// Niagara와 BoxCollision이 있는 객체를 생성 후 소멸한다.
	AExplosion* pExplosion = m_SkillUser->GetWorld()->
		SpawnActorDeferred<AExplosion>(
			ExplosionClass,
			FTransform(FRotator::ZeroRotator, SpawnPos),
			this,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


	pExplosion->Init(m_SkillUser, m_Target, _ForceDamage);

	pExplosion->FinishSpawning(FTransform(FRotator::ZeroRotator, SpawnPos));

	if (m_WarningSign)
		m_WarningSign->Destroy();
	Destroy();
}

void APitchProjectile::InitBeforeShoot()
{
	m_Elapsed = 0.f;

	m_StartLocation = GetActorLocation();
	m_DirVec = m_Destination - GetActorLocation();
	m_Distance = m_DirVec.Length();
	m_DirVec.Normalize();
}


