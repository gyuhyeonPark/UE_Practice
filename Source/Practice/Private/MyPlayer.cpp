// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Boxcomponent.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "InputContainer.h"
#include "PlayerSkillComponent.h"
#include "PlayerStatComponent.h"
#include "../GlobalEnum.h"

#include "../UI/UIManager.h"
#include "../UI/BattingModeWidget.h"

#include "../Item/Weapon/Weapon.h"

//TEMP
#include "../Sounds/SoundManager.h"

#include "../Actor/PoseCopy.h"
#include "../Item/InventoryManager.h"

#include "PaperSpriteComponent.h"


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
	m_SpringArm->bUsePawnControlRotation = true;
/*	bUseControllerRotationPitch = true;*/
	m_SpringArm->bDoCollisionTest = false;

	// 카메라 컴포넌트 추가
	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	m_Camera->SetupAttachment(m_SpringArm);
	m_Camera->bUsePawnControlRotation = false;

	// SkillComponent 추가하기
	m_SkillCom = CreateDefaultSubobject<UPlayerSkillComponent>(TEXT("PlayerSkillComponent"));
	m_StatCom = CreateDefaultSubobject<UPlayerStatComponent>(TEXT("StatComponent"));

	m_InteractionCom = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	m_CurrentUI = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("DisplayingUI"));
	m_CurrentUI->SetupAttachment(RootComponent);

	// 캡슐 컴포넌트 절반길이 설정
	GetCapsuleComponent()->SetCapsuleHalfHeight(95.f);

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -95.f));
	
	GetCharacterMovement()->JumpZVelocity = 0.f;

	// InputContainer 생성
	m_InputContainer = CreateDefaultSubobject<UInputContainer>(TEXT("InputContainer"));

	m_BattingModeWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BattingModeWidget"));
	m_BattingModeWidget->SetupAttachment(RootComponent);

	// 컴포넌트 생성 — 이름이 에디터에 표시됨
	StrikeZone = CreateDefaultSubobject<UBoxComponent>(TEXT("StrikeZone"));
	StrikeZone->SetupAttachment(RootComponent);
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

/*	// Game Instance 예시 - Singleton 처럼 SoundManager를 사용할 수 있다.
	UGameInstance* pGI = GetWorld()->GetGameInstance();

	// UGameInstanceSubsystem 을 상속 받았다면 GameInstance의 하위항목으로 등록되어 있을 것이다.
	USoundManager* pSoundMgr = pGI->GetSubsystem<USoundManager>();

	// SoundManager에게 재생시킬 사운드 에셋 로딩
	USoundBase* pSoundWave = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/BGM_Test.BGM_Test"));
	
	pSoundMgr->PlayBGM(pSoundWave, 2.f);*/

	// skeletal mesh의 소켓에 무기 장착해주기
	if (weaponClass)
	{
		// 무기 생성
		AWeapon* pWeapon = GetWorld()->SpawnActor<AWeapon>(weaponClass);

		if (pWeapon)
		{
			pWeapon->SetOwner(this);
			pWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				TEXT("WeaponSock"));
		}
		m_SkillCom->SetWeapon(pWeapon);
	}


	InitPostProcessMaterial();

	m_Camera->SetFieldOfView(m_NormalFOV);
	m_SpringArm->TargetArmLength = m_NormalArmLength;

	StrikeZone->SetHiddenInGame(false);        // 게임 중에도 표시
	StrikeZone->SetVisibility(true);

	m_BattingModeWidget->SetVisibility(false);
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

	// Batting Mode 바인딩
	if (const UInputAction* pAction = m_InputContainer->FindIAByName(TEXT("IA_RightClick")))
	{
		pEIC->BindAction(pAction, ETriggerEvent::Triggered, this, &AMyPlayer::RightClickInteraction);
	}

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

	StartIllusion(5.f, 0.2f);

	Super::Jump();
}

void AMyPlayer::LookAction(const FInputActionValue& _Value)
{
	if (m_CombatMode == ECombatMode::NORMAL)
	{
		// 입력 값
		const FVector2D Input = _Value.Get<FVector2D>();

		AController* pController = GetController();

		if (pController == nullptr)
		{
			return;
		}

		// 현재 Control Rotation
		FRotator ControlRot = pController->GetControlRotation();

		// Pitch 정규화
		float Pitch = FRotator::NormalizeAxis(ControlRot.Pitch);

		// 회전 누적
		ControlRot.Yaw += Input.X * m_RotateScale;

		Pitch -= Input.Y * m_RotateScale;

		// Clamp
		Pitch = FMath::Clamp(
			Pitch,
			m_MinPitch,
			m_MaxPitch
		);

		// 적용
		ControlRot.Pitch = Pitch;

		// Roll 제거
		ControlRot.Roll = 0.f;

		// 최종 반영
		pController->SetControlRotation(ControlRot);

		// 필요하다면 저장
		m_CurPitch = Pitch;
	}
	else if (m_CombatMode == ECombatMode::BATTING)
	{
		if (m_UpdateAimPos.IsBound())
		{
			APlayerController* PC = Cast<APlayerController>(GetController());

			if (PC)
			{
				float MouseX, MouseY;
				PC->GetMousePosition(MouseX, MouseY);
				m_UpdateAimPos.Broadcast(MouseX, MouseY);
			}
		}
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

void AMyPlayer::BattingModeToggle()
{
	m_CombatMode == ECombatMode::NORMAL ? EnterBattingMode() : ExitBattingMode();
}

void AMyPlayer::EnterBattingMode()
{
	m_CombatMode = ECombatMode::BATTING;
	SetMoveScale(0.f);

	m_InputContainer->SetBattingMode(GetController());

	m_SkillCom->SetSkillSlotBattingMode();

	m_BattingModeWidget->SetVisibility(true);

	// 카메라 설정 및 UI 생성
	m_BZoomElapsed = 0.f;
	m_BModePitch = GetController()->GetControlRotation().Pitch;

	GetWorldTimerManager().ClearTimer(m_BModeZoomHandle);

	GetWorldTimerManager().SetTimer(m_BModeZoomHandle,
		this,
		&AMyPlayer::BattingModeZoom,
		m_BZoomTick, true);

	if (m_InitAimPos.IsBound())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());

		if (PC)
		{
			int32 SizeX;
			int32 SizeY;

			PC->GetViewportSize(SizeX, SizeY);

			PC->SetMouseLocation(
				SizeX / 2,
				SizeY / 2);

			float MouseX, MouseY;
			PC->GetMousePosition(MouseX, MouseY);
			m_InitAimPos.Broadcast(MouseX, MouseY);
		}
	}
}

void AMyPlayer::ExitBattingMode()
{
	if (m_CombatMode == ECombatMode::NORMAL)
		return;

	m_CombatMode = ECombatMode::NORMAL;
	SetMoveScale(1.f);

	m_InputContainer->SetNormalMode(GetController());

	// 재생 중인 몽타주가 있다면 STOP. Idle로 돌아가는 걸 기대.
	GetMesh()->GetAnimInstance()->StopAllMontages(1.5f);

	m_SkillCom->SetSkillSlotNormal();
	
	m_SkillCom->EndSkill();

	m_BattingModeWidget->SetVisibility(false);

	// 카메라 설정 및 UI 생성 
	m_BZoomElapsed = 0.f;

	GetWorldTimerManager().ClearTimer(m_BModeZoomHandle);

	GetWorldTimerManager().SetTimer(m_BModeZoomHandle,
		this,
		&AMyPlayer::BattingModeUnzoom,
		m_BZoomTick, true);
}

void AMyPlayer::RightClickInteraction(const FInputActionValue& _Value)
{
	ToggleInteraction(EInteractionType::RClick);
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

USkillComponent* AMyPlayer::GetSkillComponent()
{
	return Cast<USkillComponent>(m_SkillCom);
}

void AMyPlayer::SetInteractionUISprite(UPaperSprite* _Sprite)
{
	m_CurrentUI->SetSprite(_Sprite);
}

void AMyPlayer::ChangePlayerState(EPlayerState _NextState)
{
	if (m_PlayerState == _NextState)
		return;
	if (m_PlayerState != EPlayerState::JUMP)
		m_PlayerState = _NextState;
}

void AMyPlayer::ToggleInteraction(EInteractionType _Type)
{
	m_InteractionCom->ExecuteInteract(_Type, this);
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

	TriggerHeartEffect(3.f, 1.f);

	return Damage;
}

void AMyPlayer::StartIllusion(float _Duration, float _Interval)
{
	// 일정 시간마다 호출되는 타이머 등록(분신을 생성하는 함수를 일정 간격마다 호출하게 한다
	GetWorldTimerManager().SetTimer(m_IllusionCreateHandle,
		this,
		&AMyPlayer::CreateIllusion,
		_Interval, true);

	// 지정된 시간이 끝나면, 모든 타이머를 종료시키는 타이머 등록
	GetWorldTimerManager().SetTimer(m_IllusionStopHandle,
		this,
		&AMyPlayer::StopIllusion,
		_Duration, false);
}

void AMyPlayer::CreateIllusion()
{
	// SkeletalMeshComponent의 월드 위치정보를 구한다.
	FTransform WorldTrans = FTransform(
		GetMesh()->GetComponentRotation(),
		GetMesh()->GetComponentLocation(),
		GetMesh()->GetComponentScale());

	// 지연스폰으로 일단 객체를 만든다.
	APoseCopy* pPoseCopy = GetWorld()->SpawnActorDeferred<APoseCopy>(
		APoseCopy::StaticClass(),
		WorldTrans,
		this,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// 생성된 PoseCopy 액터가 따라할 포즈를 알려준다.
	pPoseCopy->CopyPos(GetMesh(), m_IllusionMtrl, 3.f);

	// BeginPlay 호출
	pPoseCopy->FinishSpawning(WorldTrans);
}

void AMyPlayer::StopIllusion()
{
	GetWorldTimerManager().ClearTimer(m_IllusionCreateHandle);
	GetWorldTimerManager().ClearTimer(m_IllusionStopHandle);
}

void AMyPlayer::TriggerHeartEffect(float _Duration, float _MaxWeight)
{
	FWeightedBlendable& Pair = m_Camera->PostProcessSettings.WeightedBlendables.Array[0];
	Pair.Weight = 1.f;

	m_HEElapsed = 0.f;
	m_HEDuration = _Duration;
	m_HEMaxWeight = _MaxWeight;

	// 일정 시간마다 호출되는 타이머 등록
	GetWorldTimerManager().SetTimer(m_HEHandle,
		this,
		&AMyPlayer::HeartEffectUpdate,
		0.1f, true);
}

void AMyPlayer::HeartEffectUpdate()
{
	m_HEElapsed += 0.1f;
	FWeightedBlendable& Pair = m_Camera->PostProcessSettings.WeightedBlendables.Array[0];

	if (m_HEElapsed >= m_HEDuration)
	{
		Pair.Weight = 0.f;
		GetWorldTimerManager().ClearTimer(m_HEHandle);
	}
	else
	{
		Pair.Weight = 1.f;

		float halfTime = m_HEDuration / 2.f;

		float curAlpha = 0.f;

		if (m_HEElapsed < halfTime)
		{
			curAlpha = FMath::Lerp(
				0.f,
				m_HEMaxWeight,
				m_HEElapsed / halfTime);
		}
		else
		{
			curAlpha = FMath::Lerp(
				0.f,
				m_HEMaxWeight,
				(m_HEElapsed - halfTime) / halfTime);
		}

		// 재질 효과 off
		if (UMaterialInstanceDynamic* pMDI = Cast<UMaterialInstanceDynamic>(Pair.Object))
		{
			pMDI->SetScalarParameterValue(TEXT("BlendWeight"), curAlpha);
		}
	}
}

void AMyPlayer::BattingModeZoom()
{
	m_BZoomElapsed += m_BZoomTick;

	if (m_BZoomElapsed >= m_BZoomDuration)
	{
		GetWorldTimerManager().ClearTimer(m_HEHandle);
	}
	else
	{
		float ratio = m_BZoomElapsed / m_BZoomDuration;
		float EaseRatio = FMath::InterpEaseOut(0.f, 1.f, ratio, m_EasingStrength);

		// FOV
		float curAlpha = 0.f;
		curAlpha = FMath::Lerp(m_NormalFOV, m_BModeFOV, EaseRatio);
		m_Camera->SetFieldOfView(curAlpha);
		
		// SpringArm Length
		curAlpha = FMath::Lerp(m_NormalArmLength, m_BModeArmLength, EaseRatio);
		m_SpringArm->TargetArmLength = curAlpha;

		// SpringArm Offset
		FVector pCamOffset = {};

		curAlpha = FMath::Lerp(0.f, m_BModeCamOffset.Y, EaseRatio);
		pCamOffset.Y = curAlpha;

		curAlpha = FMath::Lerp(0.f, m_BModeCamOffset.Z, EaseRatio);
		pCamOffset.Z = curAlpha;
		
		m_SpringArm->SocketOffset = pCamOffset;
	
		// Pitch 0으로 보간.
		FRotator ControlRot = GetController()->GetControlRotation();
		curAlpha = FMath::Lerp(m_BModePitch, 0.f, EaseRatio);
		ControlRot.Pitch = curAlpha;

		GetController()->SetControlRotation(ControlRot);
	}
}

void AMyPlayer::BattingModeUnzoom()
{
	m_BZoomElapsed += m_BZoomTick;

	if (m_BZoomElapsed >= m_BZoomDuration)
	{
		GetWorldTimerManager().ClearTimer(m_HEHandle);
	}
	else
	{
		m_SpringArm->bUsePawnControlRotation = true;

		float ratio = m_BZoomElapsed / m_BZoomDuration;

		float EaseRatio = FMath::InterpEaseOut(0.f, 1.f, ratio, m_EasingStrength);

		float curAlpha = 0.f;
		curAlpha = FMath::Lerp(m_BModeFOV, m_NormalFOV, EaseRatio);
		m_Camera->SetFieldOfView(curAlpha);

		// SpringArm Length
		curAlpha = FMath::Lerp(m_BModeArmLength, m_NormalArmLength, EaseRatio);
		m_SpringArm->TargetArmLength = curAlpha;

		// SpringArm Offset
		FVector pCamOffset = {};

		curAlpha = FMath::Lerp(m_BModeCamOffset.Y, 0.f, EaseRatio);
		pCamOffset.Y = curAlpha;

		curAlpha = FMath::Lerp(m_BModeCamOffset.Z, 0.f, EaseRatio);
		pCamOffset.Z = curAlpha;

		m_SpringArm->SocketOffset = pCamOffset;

		// Unzoom에선 Pitch 조절 X
	}
}

void AMyPlayer::BatImpact()
{
	if (UBattingModeWidget* pBModeWidget = Cast<UBattingModeWidget>(m_BattingModeWidget->GetUserWidgetObject()))
	{
		pBModeWidget->Impact();
	}
}

void AMyPlayer::SendChargeElapsed(float _Elapsed) const
{
	if (UBattingModeWidget* pBModeWidget = Cast<UBattingModeWidget>(m_BattingModeWidget->GetUserWidgetObject()))
	{
		pBModeWidget->SetChargeElapsed(_Elapsed);
	}
}

void AMyPlayer::InitPostProcessMaterial()
{
	if (!m_Camera)
		return;

	m_Camera->PostProcessBlendWeight = 1.f;

	// 각 포스트 프로세스 원본재질의 사본.
	// 동적재질을 미리 만들어줌.
	for (auto& Blendable : m_Camera->PostProcessSettings.WeightedBlendables.Array)
	{
		// 원래 재질은 원본을 가리키고 있는데,
		// 이를 동적재질(사본)으로 교체해준다.
		if (UMaterialInterface* pMtrl = Cast<UMaterialInterface>(Blendable.Object))
		{
			UMaterialInstanceDynamic* pMtrlDynamic = UMaterialInstanceDynamic::Create(pMtrl, this);

			Blendable.Object = pMtrlDynamic;
		}

		Blendable.Weight = 0.f;
	}
}

bool AMyPlayer::Server_RequestPickupItem_Validate(AActor* _FieldItem)
{
	return true;
}

void AMyPlayer::Server_RequestPickupItem_Implementation(AActor* _FieldItem)
{
	// 아이템 유효성 체크
	if (!IsValid(_FieldItem))
		return;

	// bPickup 체크
	UClass* FieldItemClass = _FieldItem->GetClass();
	FProperty* PickupProp = FieldItemClass->FindPropertyByName(TEXT("bPickup"));
	FBoolProperty* BoolProp = CastField<FBoolProperty>(PickupProp);

	bool bPickup = BoolProp->GetPropertyValue_InContainer(_FieldItem);

	if (bPickup)
		return;

	// bPickup을 true로 변경 (다른 유저가 못 먹게)
	BoolProp->SetPropertyValue_InContainer(_FieldItem, true);

	UDataTable* pTable = nullptr;
	FName RowName;
	int32 Count = 0;

	// _FieldItem으로부터 DataTable, RowName, Count를 가져온다.
	FProperty* DataTableProp = FieldItemClass->FindPropertyByName(TEXT("DataTable"));
	if (DataTableProp)
	{
		if (FObjectProperty* ObjProp = CastField<FObjectProperty>(DataTableProp))
		{
			pTable = Cast<UDataTable>(ObjProp->GetObjectPropertyValue_InContainer(_FieldItem));
		}
	}

	FProperty* NameProp = FieldItemClass->FindPropertyByName(TEXT("RowName"));
	if (NameProp)
	{
		if (FNameProperty* Prop = CastField<FNameProperty>(NameProp))
		{
			RowName = Prop->GetPropertyValue_InContainer(_FieldItem);
		}
	}

	FProperty* IntProp = FieldItemClass->FindPropertyByName(TEXT("Count"));
	if (IntProp)
	{
		if (FIntProperty* Prop = CastField<FIntProperty>(IntProp))
		{
			Count = Prop->GetPropertyValue_InContainer(_FieldItem);
		}
	}

	// 클라이언트에 승인 RPC 보냄
	Client_PickupSuccess(pTable, RowName, Count);

	// 필드배치 아이템 삭제
	_FieldItem->Destroy();
}

void AMyPlayer::Client_PickupSuccess_Implementation(UDataTable* _Table, FName _RowName, int32 _Count)
{
	// 인벤토리 매니저에 아이템을 추가한다.
	if (UInventoryManager* pMgr = GetGameInstance()->GetSubsystem<UInventoryManager>())
		pMgr->AddItem(_Table, _RowName);
}

