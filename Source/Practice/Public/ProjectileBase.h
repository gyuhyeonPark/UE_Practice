// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class PRACTICE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void DetectCollision();

public:
	void InitProjectile(APawn* _User, class USkillDataBase* _Skill)
	{
		m_SkillUser = _User;
		m_Skill = _Skill;
	}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Collision")
	bool IsHit(AActor* _HitActor);
	virtual bool IsHit_Implementation(AActor* _HitActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Collision")
	void OnHit(AActor* _HitActor, const FHitResult& _Result);
	virtual void OnHit_Implementation(AActor* _HitActor, const FHitResult& _Result);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* m_Sphere;			// PMC의 물리 시뮬레이션을 위해 추가

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UProjectileMovementComponent* m_PMC;	// 투사체 움직임 제어 컴포넌트

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UNiagaraComponent* m_NiagaraCom;		// 나이아가라 재생 컴포넌트

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Niagara")
	class UNiagaraSystem* m_ProjectileEffect;	// 투사체 시각효과 이펙트

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Niagara")
	class UNiagaraSystem* m_HitEffect;			// 투사체 히트 시 발생할 이펙트


	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Info")
	float m_ProjectileRadius;


	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Info")
	int32 m_MaxHitCount;	// 투사체 최대 히트 수
	int32 m_CurHitCount;	// 현재 충돌 횟수

	FVector m_PrevPos;							// 이전 위치 기억

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	APawn* m_SkillUser;							// 투사체 생성자

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	class USkillDataBase* m_Skill;				// 투사체를 생성시킨 스킬
	
	TSet<TWeakObjectPtr<AActor>> m_HitActors;	// 중복 충돌 방지, 충돌 대상 기억
};
