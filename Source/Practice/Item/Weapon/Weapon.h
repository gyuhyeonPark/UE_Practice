// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

// 해당 클래스를 상속받아 근거리 - 원거리 무기로 분기하면 좋지 않을까?
// 무기에 따른 애니메이션 또한 바인딩할 생각까지 해야겠다...
UCLASS()
class PRACTICE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FVector GetHitSockPos() { return weaponMesh->GetSocketLocation(TEXT("HitBoxSock")); }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	UStaticMeshComponent* weaponMesh;
};
