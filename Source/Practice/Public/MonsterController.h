// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterController.generated.h"

USTRUCT(BlueprintType)
struct FSensedTargetInfo
{
	GENERATED_BODY();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<AActor> Target;

	// 어그로 수치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AggroValue = 0.f;

	// 인지 유무
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bSensed;

	// 마지막으로 확인된 위치
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector LosePos;

	// 인지를 놓친 시간
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float LoseTime;
};

UCLASS()
class PRACTICE_API AMonsterController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterController();

public:
	UFUNCTION()
	void OnTargetDetected(AActor* _Target, FAIStimulus _Stimulus);

	const TArray<FSensedTargetInfo>& GetSensedTargets() { return m_SensedTargets; }

	FSensedTargetInfo& AddSensedTarget(AActor* _Target);
	FSensedTargetInfo* FindSensedTarget(const AActor* _Target);
	void ExpireSensedTarget(float _LimintTime);

protected:
	virtual void OnPossess(APawn* _Pawn) override;

protected:
	UPROPERTY()
	class UAISenseConfig_Sight* m_SightConfig;

	UPROPERTY()
	class UAISenseConfig_Damage* m_DamageConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UAIPerceptionComponent* m_PerceptionCom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBehaviorTree* m_BTAsset;		// 사용할 행동 트리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UBlackboardData* m_BBAsset;	// 데이터를 기록할 블랙보드 에셋

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TArray<FSensedTargetInfo>	m_SensedTargets;
};
