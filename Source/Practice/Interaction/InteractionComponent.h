// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactable.h"
#include "InteractionComponent.generated.h"

/*
* Player가 상호작용 키(ex. 마우스 우클릭, E)를 눌렀을 때,
* IInteractable
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICE_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void RegisterInteractable(class IInteractable* _Interactable);
	void DeregisterInteractable(class IInteractable* _Interactable);

public:
	void ExecuteInteract(EInteractionType _KeyType, class AMyPlayer* _Player);

public:
	TMap<EInteractionType, TObjectPtr<class UPaperSprite>>& GetUISpriteMap() { return m_UISprites; }

protected:
	TMap<EInteractionType, TArray<IInteractable*>> m_Interactables;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TMap<EInteractionType, TObjectPtr<class UPaperSprite>> m_UISprites;
};
