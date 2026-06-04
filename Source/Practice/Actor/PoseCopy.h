// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PoseCopy.generated.h"

UCLASS()
class PRACTICE_API APoseCopy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoseCopy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void CopyPos(class USkeletalMeshComponent* _Com, class UMaterialInterface* _UseMtrl, float _Duration);

protected:
	class UPoseableMeshComponent* m_MeshCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material", meta = (ExposeOnSpawn = "true"))
	float m_Duration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	float m_Elapsed;

};
