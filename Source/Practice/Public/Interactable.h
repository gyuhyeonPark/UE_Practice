// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	RClick,
	E,
	END UMETA(Hidden),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PRACTICE_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanInteract() const = 0;
	virtual EInteractionType GetInteractionType() const = 0;
	virtual void Interact(class AMyPlayer* Player) = 0;
	virtual void CancleInteract(class AMyPlayer* Player) = 0;
};
