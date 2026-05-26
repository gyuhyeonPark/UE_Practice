// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Boxcomponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputContainer.h"
#include "SkillComponent.h"
#include "PlayerStatComponent.h"
#include "../GlobalEnum.h"

#include "../UI/UIManager.h"

//TEMP
#include "../Sounds/SoundManager.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SkeletalMeshComponent 에 Mesh 설정하기
	if (GetMesh())
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Finder(TEXT("/Game/ParagonSunWukong/Characters/Heroes/Wukong/Skins/GreatSage/Meshes/Wukong_GreatSage.Wukong_GreatSage"));
		
		if (Finder.Succeeded())
			GetMesh()->SetSkeletalMesh(Finder.Object);
	}
	
	// SpringArm Component 추가
	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	m_SpringArm->SetupAttachment(RootComponent);
	m_SpringArm->TargetArmLength = 250.f;
	m_SpringArm->bUsePawnControlRotation = true;

	// 카메라 컴포넌트 추가
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	m_Camera->SetupAttachment(m_SpringArm);

	// SkillComponent 추가하기
	m_SkillCom = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
		
	m_StatCom = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("StatComponent"));

	// 캡슐 컴포넌트 절반길이 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(95.f);

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	
	GetCharacterMovement()->JumpZVelocity = 0.f;

	// InputContainer 생성
	m_InputContainer = CreateDefaultSubobject<UInputContainer>(TEXT("InputContainer"));
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	m_BaseMaxSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// Tag
	Tags.Add(TEXT("Player"));

	// TeamId
	SetGenericTeamId((uint8)ETeamType::Player);

	// Enum값을 문자열로 전환하는 방법
/*	FString Name;
	Name = StaticEnum<EPlayerState>()->GetNameStringByValue((int64)m_PlayerState);
	FText text = StaticEnum<EPlayerState>()->GetDisplayNameTextByValue((int64)m_PlayerState);

	// 언리얼에서 사용하는 문자열 타입
	FString;	// 일반적인 문자열 컨테이너. 동적할당으로 문자 관리
	FName;		// 비교에 최적화된 문자열, 실제로는 전역공간에 저장된 문자열의 인덱스(숫자)를 기록하고 있다.
	FText;		// 현지화

	FText SkillName = NSLOCTEXT("SkillData", "FIREBALL_DESC", "화염구");*/

	// Game Instance 예시 - Singleton 처럼 SoundManager를 사용할 수 있다.
	UGameInstance* pGI = GetWorld()->GetGameInstance();

	// UGameInstanceSubsystem 을 상속 받았다면 GameInstance의 하위항목으로 등록되어 있을 것이다.
	USoundManager* pSoundMgr = pGI->GetSubsystem<USoundManager>();

	// SoundManager에게 재생시킬 사운드 에셋 로딩
	USoundBase* pSoundWave = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/BGM_Test.BGM_Test"));
	
	pSoundMgr->PlayBGM(pSoundWave, 2.f);
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 점프 쿨타임 적용
	if (m_JumpLock)
	{
		m_JumpCurLockTime += DeltaTime;
		if (m_JumpCurLockTime >= m_JumpLockTime)
		{
			m_JumpLock = false;
		}
	}

	if (GetCharacterMovement()->Velocity.Size2D() < 10.f)
		ChangePlayerState(EPlayerState::IDLE);
	else
		ChangePlayerState(EPlayerState::MOVE);
}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	m_InputContainer->Init(GetController());

	// InputComponent에 특정 InputAction이 발생했을 때 호출시킬 함수 Delegate
	UEnhancedInputComponent* pEIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (const UInputAction* pAction = m_InputContainer->FindIAByName(TEXT("IA_Move")))
		pEIC->BindAction(pAction, ETriggerEvent::Triggered, this, &AMyPlayer::MoveAction);

	if (const UInputAction* pAction = m_InputContainer->FindIAByName(TEXT("IA_Look")))
		pEIC->BindAction(pAction, ETriggerEvent::Triggered, this, &AMyPlayer::LookAction);

	if (const UInputAction* pAction = m_InputContainer->FindIAByName(TEXT("IA_Jump")))
		pEIC->BindAction(pAction, ETriggerEvent::Triggered, this, &AMyPlayer::JumpAction);

	if (const UInputAction* pAction = m_InputContainer->FindIAByName(TEXT("IA_Sprint")))
	{
		pEIC->BindAction(pAction, ETriggerEvent::Triggered, this, &AMyPlayer::SprintTriggered);
		pEIC->BindAction(pAction, ETriggerEvent::Completed, this, &AMyPlayer::SprintCompleted);
	}

	if (const UInputAction* pAction = m_InputContainer->FindIAByName(TEXT("IA_InvenToggle")))
		pEIC->BindAction(pAction, ETriggerEvent::Started, this, &AMyPlayer::InvenToggle);


	// 스킬 컴포넌트가 담당하는 키 입력에 대해서는 해당 클래스에 바인딩 시킨다.
	m_SkillCom->Bind(pEIC, m_InputContainer);
}

void AMyPlayer::MoveAction(const FInputActionValue& _Value)
{
	FVector2D Input = _Value.Get<FVector2D>();

	FVector vF = GetActorForwardVector();
	FVector vR = GetActorRightVector();

	AddMovementInput(vF, Input.X);
	AddMovementInput(vR, Input.Y);

	//UE_LOG(LogTemp, Warning, TEXT("MoveAction Call"));
}

void AMyPlayer::JumpAction(const FInputActionValue& _Value)
{
	if (m_JumpLock)
		return;

	ChangePlayerState(EPlayerState::JUMP);

	Super::Jump();
}

void AMyPlayer::LookAction(const FInputActionValue& _Value)
{
	// 1. 마우스 입력 값
	FVector2D input = _Value.Get<FVector2D>();

	// 2. 컨트롤러를 회전 시킨다.
	if (GetController() != nullptr)
	{
		AddControllerYawInput(input.X * m_RotateScale);

		// 현재 회전 상태 가져오기
		FRotator curRotate = GetControlRotation();

		// 현재 Pitch 회전에 마우스 입력값을 누적한다.
		float newPitch = input.Y + curRotate.Pitch;

		// 각도 범위를 0 ~ 360 -> -180 ~ 180으로 정규화
		newPitch = FMath::Clamp(newPitch, -m_MaxPitch, m_MaxPitch);
		
		// 더한 값으로 Pitch 수정
		curRotate.Pitch = newPitch;

		// 회전에 적용하기
		GetController()->SetControlRotation(curRotate);
	}
}

void AMyPlayer::SprintTriggered(const FInputActionValue& _Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 1500.f;
}

void AMyPlayer::SprintCompleted(const FInputActionValue& _Value)
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMyPlayer::InvenToggle(const FInputActionValue& _Value)
{
	// Inventory 위젯 토글
	AUIManager* pUIMgr = Cast<AUIManager>(Cast<APlayerController>(GetController())->GetHUD());

	pUIMgr->ToggleInventory();
}

ETeamAttitude::Type AMyPlayer::GetTeamAttitudeTowards(const AActor& Other) const
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

void AMyPlayer::ChangePlayerState(EPlayerState _NextState)
{
	if (m_PlayerState == _NextState)
		return;
	if (m_PlayerState != EPlayerState::JUMP)
		m_PlayerState = _NextState;
}

float AMyPlayer::TakeDamage(float _Damage, FDamageEvent const& _DamageEvent, AController* _InstigatorController, AActor* _InstigatorActor)
{
	float Damage = Super::TakeDamage(_Damage, _DamageEvent, _InstigatorController, _InstigatorActor);

	float CurHP = m_StatCom->GetStat(TEXT("CurHP"));
	float MaxHP = m_StatCom->GetStat(TEXT("MaxHP"));
	
	CurHP -= Damage;

	if (CurHP < 0.f)
		CurHP = 0.f;

	m_StatCom->SetStat(TEXT("CurHP"), CurHP);

	// 데미지 받았을 때 호출받고 싶은 델리게이트가 등록된 게 있다면, 호출해주기
	if (m_OnTakeDamage.IsBound())
	{
		m_OnTakeDamage.Broadcast(CurHP, MaxHP);
	}

	return Damage;
}

