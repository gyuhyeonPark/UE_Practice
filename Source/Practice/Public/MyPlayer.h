// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "MyPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage, float, CurHP, float, MaxHP);

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

public:
	void MoveAction(const struct FInputActionValue& _Value);
	void JumpAction(const struct FInputActionValue& _Value);
	void LookAction(const struct FInputActionValue& _Value);
	void SprintTriggered(const struct FInputActionValue& _Value);
	void SprintCompleted(const struct FInputActionValue& _Value);
	void InvenToggle(const struct FInputActionValue& _Value);

	void JumpLock()
	{
		m_JumpLock = true;
		m_JumpCurLockTime = 0.f;
	}

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
	class USkillComponent* GetSkillComponent() { return m_SkillCom; }

	void SetMoveScale(float _Scale)
	{
		m_MoveScale = _Scale;
		GetCharacterMovement()->MaxWalkSpeed = m_BaseMaxSpeed * m_MoveScale;
	}

	void SetRotateScale(float _Scale) { m_RotateScale = _Scale; }

	UFUNCTION()
	void ChangePlayerState(EPlayerState _NextState);

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "SpringArm"))
	class USpringArmComponent* m_SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "MainCamera"))
	class UCameraComponent* m_Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "SkillComponent"))
	class USkillComponent* m_SkillCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "StatComponent"))
	class UPlayerStatComponent* m_StatCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"), Instanced)
	class UInputContainer* m_InputContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch")
	int32 m_MaxPitch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pitch")
	int32 m_CurPitch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "JumpLockTime"))
	float m_JumpLockTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimData")
	UAnimMontage* m_TestMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status")
	EPlayerState m_PlayerState;

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
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> weaponClass;

private:
	bool m_JumpLock;
	float m_JumpCurLockTime;

public:
	FOnTakeDamage m_OnTakeDamage;
};
