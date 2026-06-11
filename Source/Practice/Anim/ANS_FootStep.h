// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_FootStep.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UANS_FootStep : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	void PlayFootstep(class AActor* _Owner, USkeletalMeshComponent* _MeshCom);

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName m_SockName;
};
