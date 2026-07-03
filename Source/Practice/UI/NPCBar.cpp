// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/NPCBar.h"
#include "Components/ProgressBar.h"
#include "StatComponent.h"

void UNPCBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UNPCBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UNPCBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UNPCBar::UpdateHPBar(UStatComponent* _StatCom)
{
	m_HPBar->SetPercent(_StatCom->GetStat(TEXT("CurHP")) / _StatCom->GetStat(TEXT("MaxHP")));
}
