 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Interaction/InteractionComponent.h"
#include "../GlobalEnum.h"
#include "MyPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage, float, CurHP, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateAimPos, float, XPos, float, YPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInitAimPos, float, XPos, float, YPos);

UENUM(BlueprintType)
enum class ECombatMode : uint8
{
	NORMAL,
	BATTING,
	END UMETA(Hidden),
};

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	IDLE UMETA(DisplayName = "Test"),
	MOVE,
	DASH,
	UseSkill,
	UseMoveSkill,
	JUMP,
	LAND,

	END UMETA(Hidden),
};

UCLASS()
class PRACTICE_API AMyPlayer : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void MoveAction(const struct FInputActionValue& _Value);
	void JumpAction(const struct FInputActionValue& _Value);
	void LookAction(const struct FInputActionValue& _Value);
	void SprintTriggered(const struct FInputActionValue& _Value);
	void SprintCompleted(const struct FInputActionValue& _Value);
	void InvenToggle(const struct FInputActionValue& _Value);
	void RightClickInteraction(const struct FInputActionValue& _Value);

	// Generic Team Agent Interface abstract functions
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) { m_TeamId = TeamID; }

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const { return m_TeamId; }

	/** Retrieved owner attitude toward given Other object */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	EPlayerState GetPlayerState() { return m_PlayerState; }
	ECombatMode GetCombatMode() { return m_CombatMode; }

	class USkillComponent* GetSkillComponent();

	class UInteractionComponent* GetInteractionComponent() { return m_InteractionCom; }

	class UPaperSpriteComponent* GetCurrentUISpriteCom() { return m_CurrentUI; }

	void SetInteractionUISprite(class UPaperSprite* _Sprite); 

	void SetMoveScale(float _Scale)
	{
		m_MoveScale = _Scale;
		GetCharacterMovement()->MaxWalkSpeed = m_BaseMaxSpeed * m_MoveScale;
	}

	void SetRotateScale(float _Scale) { m_RotateScale = _Scale; }

	UFUNCTION()
	void ChangePlayerState(EPlayerState _NextState);

	void JumpLock()
	{
		m_JumpLock = true;
		m_JumpCurLockTime = 0.f;
	}

	void BattingModeToggle();
	void ToggleInteraction(EInteractionType _Type);
	void EnterBattingMode();
	void ExitBattingMode();

	bool IsStun() { return m_IsStun; } const
	void EndStun() { m_IsStun = false; SetMoveScale(1.f); }

	// Take Damage
public:
	virtual float TakeDamage(float _Damage, struct FDamageEvent const& _DamageEvent,
		class AController* _InstigatorController, AActor* _InstigatorActor) override;

	// Illusion
public:
	UFUNCTION(BlueprintCallable)
	void StartIllusion(float _Duration, float _Interval);

	void CreateIllusion();
	void StopIllusion();

	UFUNCTION(BlueprintCallable)
	void TriggerHeartEffect(float _Duration, float _MaxWeight);
	void HeartEffectUpdate();

	void BattingModeZoom();
	void BattingModeUnzoom();

	void BatImpact();
	void SendChargeElapsed(float _Elapsed) const;
	
	void Parry(EParryJudgementType _ParryType);

	void SwitchWeaponHand(bool _IsRight);

public:
	void InitPostProcessMaterial();

	// Item PickUp RPC
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_RequestPickupItem(AActor* _FieldItem);

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_PickupSuccess(UDataTable* _Table, FName _RowName, int32 _Count);

	class UBoxComponent* GetStrikeZone() { return StrikeZone; }
	class UWidgetComponent* GetBattingModeWidget() { return m_BattingModeWidget; }

	bool IsParrying() const { return m_IsParrying; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "SpringArm"))
	class USpringArmComponent* m_SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "MainCamera"))
	class UCameraComponent* m_Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "PlayerSkillComponent"))
	class UPlayerSkillComponent* m_SkillCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "StatComponent"))
	class UPlayerStatComponent* m_StatCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), Instanced)
	class UInputContainer* m_InputContainer;

	// C++ 헤더에 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch")
	int32 m_MaxPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch")
	int32 m_MinPitch;

	float m_CameraYaw = 0.f;
	float m_CameraPitch = -20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "JumpLockTime"))
	float m_JumpLockTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	EPlayerState m_PlayerState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	ECombatMode m_CombatMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float m_BaseMaxSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float m_MoveScale;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	float m_RotateScale;

	FGenericTeamId m_TeamId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Illusion")
	class UMaterialInterface* m_IllusionMtrl;

	FTimerHandle m_IllusionCreateHandle;
	FTimerHandle m_IllusionStopHandle;

protected:
	// Hit Related
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* m_DamagedMontage;	// 스킬 모션

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	float m_DamagedMontageSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	UAnimMontage* m_StunMontage;	// 스킬 모션

	bool m_IsStun;

	FTimerHandle m_HEHandle;
	float m_HEElapsed;
	float m_HEDuration;
	float m_HEMaxWeight;

protected:
	// Batting Mode Camera Move
	FTimerHandle m_BModeZoomHandle;
	float m_BZoomElapsed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMode")
	float m_BZoomDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMode")
	float m_BZoomTick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMode")
	float m_NormalFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMode")
	float m_BModeFOV;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMode")
	float m_BModeArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMode")
	float m_NormalArmLength;

	// Zoom 시작 시점의 카메라 Pitch
	float m_BModePitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMode")
	float m_EasingStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMode")
	FVector m_NormalCamOffset;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "CamPivot"))
	USceneComponent* CamPivot;

	FRotator m_InitialCamRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BattingMode")
	FVector m_BModeCamOffset;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "BattingModeWidget"))
	class UWidgetComponent* m_BattingModeWidget;

	bool m_IsParrying;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> weaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBoxComponent* StrikeZone;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "InteractionComponent"))
	class UInteractionComponent* m_InteractionCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "DisplayingUI"))
	class UPaperSpriteComponent* m_CurrentUI;
private:
	bool m_JumpLock;
	float m_JumpCurLockTime;

public:
	FOnTakeDamage m_OnTakeDamage;
	FUpdateAimPos m_UpdateAimPos;
	FInitAimPos m_InitAimPos;
};
