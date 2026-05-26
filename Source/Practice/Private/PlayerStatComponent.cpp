// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"
#include "GlobalData.h"

UPlayerStatComponent::UPlayerStatComponent()
{
}

void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	InitStat();
}

void UPlayerStatComponent::InitStat()
{
	// 테이블과 행이 설정되어 있어야 한다.
	if (m_Table == nullptr || m_RowName.IsNone())
		return;

	// 모든 스탯을 다 지운다.
	m_Stats.Empty();

	FPlayerStatData* pPlayerStat = m_Table->FindRow<FPlayerStatData>(m_RowName, TEXT("PlayerStat"));

	// 데이터를 구성하고 있는 멤버들의 멤버 변수명 자체를 키값으로 해서 수치를 기록.
	InitStatFromStruct(FPlayerStatData::StaticStruct(), pPlayerStat);

	// 현재 궁극기 수치
	AddStat(TEXT("CurUP"), 0.f);

	// 부모 함수 호출
	Super::InitStat();
}
