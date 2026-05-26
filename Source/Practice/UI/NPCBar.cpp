// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/NPCBar.h"
#include "Components/ProgressBar.h"

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

void UNPCBar::UpdateHPBar(float _CurHP, float _MaxHP)
{
	m_HPBar->SetPercent(_CurHP / _MaxHP);
}
