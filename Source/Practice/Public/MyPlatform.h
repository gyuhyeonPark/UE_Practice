// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MyPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API AMyPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	AMyPlatform();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platform")
	bool IsScored;

	UFUNCTION()
	void OnPlatformHit(UPrimitiveComponent* _HitComponent,
		AActor* _OtherActor, UPrimitiveComponent* _OtherComp,
		FVector _NormalImpulse, const FHitResult& _Hit);

	
};
