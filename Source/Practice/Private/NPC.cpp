// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "SkillComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "StatComponent.h"
#include "../UI/NPCBar.h"

// Damage Config
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"

#include "Engine/DamageEvents.h"
#include "../GlobalEnum.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SkillComponent 추가하기
	m_SkillCom = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));

	// 위젯 컴포넌트 추가하기
	m_WidgetCom = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	m_WidgetCom->SetupAttachment(RootComponent);

	// 이 NPC 네비게이션 메시 생성에 영향을 주지 않도록 한다.
	SetCanAffectNavigationGeneration(false);

	// 캡슐 컴포넌트도 충돌체 이기 때문에, 확실하게 보장하기
	GetCapsuleComponent()->SetCanEverAffectNavigation(false);
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	if (m_WidgetCom)
		m_WidgetCom->SetVisibility(false);
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ANPC::TakeDamage(float _Damage, FDamageEvent const& _DamageEvent, AController* _InstigatorController, AActor* _InstigatorActor)
{
	// Super의 TakeDamage에는 여러가지 기능이 있다.
	// ex) 캐릭터의 무적 판정, 피격시 호출될 Delegate 등등...
	float fDamage = Super::TakeDamage(_Damage, _DamageEvent, _InstigatorController, _InstigatorActor);

	// 1. Damage 공식 적용하여 현재 체력에 반영하기
	if (m_NPCStatCom)
	{
		float CurHP = m_NPCStatCom->GetStat(TEXT("CurHP"));
		float MaxHP = m_NPCStatCom->GetStat(TEXT("MaxHP"));

		CurHP -= fDamage;

		if (CurHP < 0.f)
			CurHP = 0.f;

		m_NPCStatCom->SetStat(TEXT("CurHP"), CurHP);

		if (m_OnTakeDamage.IsBound())
			m_OnTakeDamage.Broadcast();

	}


	// 2. 사용중인 스킬이 있으면 스킬을 취소한다.
	USkillComponent* pSkillCom = GetComponentByClass<USkillComponent>();
	if (pSkillCom == nullptr)
		return fDamage;
	
	pSkillCom->CancleCurSkill();

	if (m_DamagedMontage != nullptr)
		GetMesh()->GetAnimInstance()->Montage_Play(m_DamagedMontage, m_DamagedMontageSpeed);

	if (_DamageEvent.DamageTypeClass == UExplosionDamageType::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("!!!Expolosion Damaged!!!"));
	}

	// 3. PerceptionComponent에게 데미지 받은걸 보고하기
	FVector InstigatorPos = _InstigatorActor ? _InstigatorActor->GetActorLocation() : GetActorLocation();

	UAISense_Damage::ReportDamageEvent(GetWorld(),
										this,
										_InstigatorActor,
										fDamage,
										InstigatorPos,
										GetActorLocation());

	// 4. 받은 데미지를 WidgetComponent를 활용해서 캐릭터 위에 띄운다.
	if (m_DamageClass)
	{
		FVector SpawnPos = GetActorLocation() + FVector(0.f, 0.f, 200.f);

		SpawnPos.X += FMath::FRandRange(-20.f, 20.f);
		SpawnPos.Y += FMath::FRandRange(-20.f, 20.f);

		// DamageActor에게 데미지값을 전달하고 나서 BeginPlay가 호출되어야 하기 때문에,
		// 지연 생성 함수를 사용한다.
		AActor* pSpawnActor = GetWorld()
			->SpawnActorDeferred<AActor>(m_DamageClass,
			FTransform(FRotator::ZeroRotator, SpawnPos),
				this,
				nullptr, 
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		// 생성된 데미지 액터에게 띄울 숫자를 알려준다.
		// Reflection을 활용. 블루프린트의 Damage 변수에 값 직접 주입
		FProperty* DamageProp = pSpawnActor->GetClass()->FindPropertyByName(TEXT("Damage"));
		if (FNumericProperty* NumericProp = CastField<FNumericProperty>(DamageProp))
		{
			void* ValuePtr = DamageProp->ContainerPtrToValuePtr<void>(pSpawnActor);
			NumericProp->SetFloatingPointPropertyValue(ValuePtr, fDamage);
		}

		// BeginPlay 호출
		pSpawnActor->FinishSpawning(FTransform(FRotator::ZeroRotator, SpawnPos));
	}

	return fDamage;
}

ETeamAttitude::Type ANPC::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* teamAgent = Cast<IGenericTeamAgentInterface>(&Other);

	if (teamAgent)
	{
		FGenericTeamId OtherId = teamAgent->GetGenericTeamId();

		if (GetGenericTeamId() == OtherId)
		{
			return ETeamAttitude::Friendly;
		}
		else
		{
			return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Neutral;
}

void ANPC::HideHPBar()
{
	if (m_WidgetCom)
		m_WidgetCom->SetVisibility(false);
}

void ANPC::ShowHPBar()
{
	// 타이머 등록
	// 3초 뒤에 위젯 렌더링 off
	if (m_WidgetCom)
		m_WidgetCom->SetVisibility(true);

	GetWorldTimerManager().SetTimer(m_HPBarVisibleHandle, this, &ANPC::HideHPBar, 3.f, false);
}

