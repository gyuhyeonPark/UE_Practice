// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

/*
* 우선 투사체가 터트릴 폭탄이지만...
* 추후 다른 곳에서 생성해도 된다.
*/
UCLASS()
class PRACTICE_API AExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Init(APawn* _SkillUser, APawn* _Target, bool _ForceDamage = false)
	{
		m_SkillUser = _SkillUser; m_Target = _Target; m_IsForceDamage = _ForceDamage;
	}

protected:
	UFUNCTION()
	void OnEffectFinished(UNiagaraComponent* FinishedComponent);

	UFUNCTION()
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "Collision"))
	class USphereComponent* m_SphereCollision;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UNiagaraComponent* m_NiagaraCom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UNiagaraSystem* m_HitEffect;		// 생성시킬 투사체의 UCLASS 정보를 가리킴

	float m_Damage;

	float m_Elapsed;

	APawn* m_SkillUser;
	APawn* m_Target;

	bool m_IsForceDamage;
};
