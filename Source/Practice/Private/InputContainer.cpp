// Fill out your copyright notice in the Description page of Project Settings.


#include "InputContainer.h"

#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedActionKeyMapping.h"
#include "EnhancedInputSubsystems.h"

void UInputContainer::Init(AController* _Controller)
{
	m_mapIA.Empty();

	AddInputAction(m_DefaultContext);
	AddInputAction(m_BattingModeContext);

	APlayerController* PC = Cast<APlayerController>(_Controller);

	if (PC)
	{
		ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();

		UEnhancedInputLocalPlayerSubsystem* pSubSys = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	
		pSubSys->ClearAllMappings();

		pSubSys->AddMappingContext(m_DefaultContext, 0);
	}
}

const UInputAction* UInputContainer::FindIAByName(const FString& _Name)
{
	const UInputAction** pAction = m_mapIA.Find(_Name);

	if (pAction)
		return *pAction;
	else
		return nullptr;
}

void UInputContainer::AddInputAction(UInputMappingContext* _IMC)
{
	if (!_IMC)
		return;

	for (const FEnhancedActionKeyMapping& Mapping : _IMC->GetMappings())
	{
		if (!Mapping.Action)
			continue;

		FString ActionName = Mapping.Action->GetName();

		if (!m_mapIA.Contains(ActionName))
		{
			m_mapIA.Add(ActionName, Mapping.Action);
		}
	}
}

void UInputContainer::SetBattingMode(AController* _Controller)
{
	APlayerController* PC = Cast<APlayerController>(_Controller);

	if (PC)
	{
		ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* pSubSys = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

		pSubSys->AddMappingContext(m_BattingModeContext, 10);
	}

	

}

void UInputContainer::SetNormalMode(AController* _Controller)
{
	APlayerController* PC = Cast<APlayerController>(_Controller);

	if (PC)
	{
		ULocalPlayer* LocalPlayer = PC->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* pSubSys = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		
		pSubSys->RemoveMappingContext(m_BattingModeContext);
/*		pSubSys->AddMappingContext(m_DefaultContext, 1);*/
	}
}
