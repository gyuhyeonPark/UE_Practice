// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/PlayerBar.h"
#include "Components/ProgressBar.h"

#include "MyPlayer.h"
#include "StatComponent.h"

void UPlayerBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Player Actor의 OnTakeAnyDamage 델리게이트에 특정 멤버함수를 미리 등록해둔다.
	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwningPlayerPawn()))
	{
		pPlayer->m_OnTakeDamage.AddDynamic(this, &UPlayerBar::UpdateHPBar_Alt);
	}
}

void UPlayerBar::NativeConstruct()
{
	Super::NativeConstruct();

	//m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	//m_HPBar->SetPercent(33.3f);

	// 플레이어의 초기 체력 상태를 Progress Bar에 반영하기
	if (AMyPlayer* pPlayer = Cast<AMyPlayer>(GetOwningPlayer()->GetPawn()))
	{
		if (UStatComponent* pStatCom = pPlayer->GetComponentByClass<UStatComponent>())
		{
			float MaxHP = pStatCom->GetStat(TEXT("MaxHP"));
			float CurHP = pStatCom->GetStat(TEXT("CurHP"));
			float Ratio = CurHP / MaxHP;

			m_HPBar->SetPercent(Ratio);
		}
	}
}

void UPlayerBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UPlayerBar::UpdateHPBar(AActor* _DamagedActor, float _Damage, const UDamageType* _DamageType, AController* _InstigatorController, AActor* _InstigatorActor)
{
	UStatComponent* pStatCom = _DamagedActor->GetComponentByClass<UStatComponent>();

	if (pStatCom != nullptr)
	{
		float Ratio = pStatCom->GetStat(TEXT("CurHP")) / pStatCom->GetStat(TEXT("MaxHP"));
		m_HPBar->SetPercent(Ratio);
	}

	UE_LOG(LogTemp, Warning, TEXT("!! Player Damaged !!"));
}

void UPlayerBar::UpdateHPBar_Alt(float _CurHP, float _MaxHP)
{
	m_HPBar->SetPercent(_CurHP / _MaxHP);
}

void UPlayerBar::UpdatePlayerHP(float _Ratio)
{
	m_HPBar->SetPercent(_Ratio);
}
