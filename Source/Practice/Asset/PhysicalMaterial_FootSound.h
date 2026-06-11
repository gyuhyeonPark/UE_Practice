// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PhysicalMaterial_FootSound.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UPhysicalMaterial_FootSound : public UPhysicalMaterial
{
	GENERATED_BODY()
	
public:
	class USoundBase* GetFootStepSound() { return m_FootSound; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundBase* m_FootSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* m_FootEffect;
};
