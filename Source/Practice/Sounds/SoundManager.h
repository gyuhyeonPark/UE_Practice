// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PRACTICE_API USoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBGM(class USoundBase* _Sound, float _FadeTime = 0.f);

	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopBGM(float _FadeOutTime = 0.f);

protected:
	UPROPERTY()
	class UAudioComponent* m_AudioCom;

};
