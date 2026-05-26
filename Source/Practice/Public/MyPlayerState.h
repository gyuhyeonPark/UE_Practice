// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void AddJumpScore(int32 _Amount) { m_JumpScore += _Amount; }
	int32 GetjumpScore() const { return m_JumpScore; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score", meta = (DisplayName = "JumpScore"))
	int32 m_JumpScore;
};
