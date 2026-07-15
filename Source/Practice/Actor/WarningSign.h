// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "WarningSign.generated.h"

UCLASS()
class PRACTICE_API AWarningSign : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarningSign();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void RegisterProjectile(class APitchProjectile* _Projectile) { m_OwnerProjectile = _Projectile; }
	
	void SetPercent(float _Elapsed, float _Duration);

protected:
	UFUNCTION()
	void RangeBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void RangeEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

public:
	virtual bool CanInteract() const override;

	virtual EInteractionType GetInteractionType() const override;

	virtual void Interact(class AMyPlayer* Player) override;

	virtual void CancleInteract(class AMyPlayer* Player) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "DecalComponent"))
	class UDecalComponent* m_DecalComponent;

	class UMaterialInstanceDynamic* m_MID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UBoxComponent* m_InRangeBox;

	class APitchProjectile* m_OwnerProjectile;

protected:
	bool m_IsPlayerInRange;
};
