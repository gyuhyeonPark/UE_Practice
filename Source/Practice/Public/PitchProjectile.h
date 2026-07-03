// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PitchProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInitWarningPos, FVector, Location);

UENUM(BlueprintType)
enum class EPitchType : uint8
{
	FASTBALL,
	CURVE,
	SLIDER,
	END UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FPitchData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPitchType Type = EPitchType::FASTBALL;

	UPROPERTY(EditAnywhere)
	float Speed = 1000.f; // cm/s

	UPROPERTY(EditAnywhere)
	FVector BreakForce = FVector::ZeroVector; // 매 프레임 누적할 힘 방향

	UPROPERTY(EditAnywhere)
	float BreakStartRatio = 0.4f; // 전체 비행의 몇 % 시점부터 변화 시작
};

UCLASS()
class PRACTICE_API APitchProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APitchProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void InitProjectile(APawn* _User, APawn* _Target, class USkillDataBase* _Skill);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PitchData")
	FPitchData m_PitchData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	APawn* m_SkillUser;							// 투사체 생성자

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	APawn* m_Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class USkillDataBase* m_Skill;				// 투사체를 생성시킨 스킬

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* m_BallMesh;

	class UBoxComponent* m_StrikeZone;

	FVector m_PrevLocation;

	FVector m_Destination;

	class UBallWarningWidget* m_WarningWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UBallWarningWidget> m_WidgetClass;

public:
	FInitWarningPos m_InitWarningPos;
};
