// Fill out your copyright notice in the Description page of Project Settings.


#include "../Interaction/InteractionComponent.h"
#include "PaperSpriteComponent.h"
#include "MyPlayer.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    m_Interactables.Add(EInteractionType::RClick);
    m_Interactables.Add(EInteractionType::E);
	// ...
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!m_Interactables[EInteractionType::RClick].IsEmpty())
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
        AMyPlayer* Player = Cast<AMyPlayer>(GetOwner());
        if (PC && Player)
        {
            UPaperSpriteComponent* pCurUI = Player->GetCurrentUISpriteCom();

            // 카메라의 월드 위치
            FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();

            // Widget의 월드 위치
            FVector WidgetLocation = pCurUI->GetComponentLocation();

            // Widget이 카메라를 바라보는 회전
            FRotator LookAtRot = (CameraLocation - WidgetLocation).Rotation();

            // Pitch까지 따라가면 UI가 눕는 경우가 많으므로 Yaw만 사용
            LookAtRot.Roll = 0.f;
            LookAtRot.Pitch = 0.f;
            LookAtRot.Yaw -= 90.f;

            pCurUI->SetWorldRotation(LookAtRot);
        }
    }
}

void UInteractionComponent::RegisterInteractable(IInteractable* _Interactable)
{
    if (m_Interactables[_Interactable->GetInteractionType()].IsEmpty())
    {
        // UI없었다면 띄워주기
        if (UPaperSprite* Sprite =
            m_UISprites.Find(_Interactable->GetInteractionType())->Get())
        {
            if (AMyPlayer* Player = Cast<AMyPlayer>(GetOwner()))
            {
                Player->SetInteractionUISprite(Sprite);
            }
        }
    }

    m_Interactables[_Interactable->GetInteractionType()].AddUnique(_Interactable);
}

void UInteractionComponent::DeregisterInteractable(IInteractable* _Interactable)
{
    if (!_Interactable)
    {
        return;
    }

    if (TArray<IInteractable*>* Interactables =
        m_Interactables.Find(_Interactable->GetInteractionType()))
    {
        Interactables->Remove(_Interactable);

        // 더이상 해당 KeyType에 등록된 상호작용이 없다면,
        // UI 꺼주기.
        if (Interactables->IsEmpty())
        {
            if (AMyPlayer* Player = Cast<AMyPlayer>(GetOwner()))
            {
                Player->SetInteractionUISprite(nullptr);
            }
        }
    }
}

void UInteractionComponent::ExecuteInteract(EInteractionType _KeyType, AMyPlayer* _Player)
{
    if (!m_Interactables[_KeyType].IsEmpty())
    {
        if (m_Interactables[_KeyType][0]->CanInteract())
        {
            m_Interactables[_KeyType][0]->Interact(_Player);
        }
    }
}

