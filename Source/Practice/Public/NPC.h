// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "NPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage_NPC, float, CurHP, float, MaxHP);

UCLASS()
class PRACTICE_API ANPC : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float _Damage, struct FDamageEvent const& _DamageEvent,
		class AController* _InstigatorController, AActor* _InstigatorActor) override;

public:
	class USkillComponent* GetSkillComponent() { return m_SkillCom; }
	// Generic Team Agent Interface abstract functions
public:
	/** Assigns Team Agent to given TeamID */
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID) { m_TeamId = TeamID; }

	/** Retrieve team identifier in form of FGenericTeamId */
	virtual FGenericTeamId GetGenericTeamId() const { return m_TeamId; }

	/** Retrieved owner attitude toward given Other object */
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	void HideHPBar();

protected:
	UPROPERTY(EditAnywhere, Category = "Info", meta = (DisplayName = "TeamID"))
	FGenericTeamId m_TeamId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "SkillComopnent"))
	class USkillComponent* m_SkillCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "HPBar"))
	class UWidgetComponent* m_WidgetCom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (DisplayName = "DamageClass"))
	TSubclassOf<AActor> m_DamageClass;

	class UStatComponent* m_NPCStatCom;

	// 체력바가 일정 시간 듸에 꺼지도록 하는 타이머
	FTimerHandle m_HPBarVisibleHandle;

	// 몬스터가 데미지 받으면 호출해줄 델리게이트
	FOnTakeDamage_NPC m_OnTakeDamage;
};
