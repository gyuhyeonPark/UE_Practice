// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterStatComponent.h"
#include "GlobalData.h"

UMonsterStatComponent::UMonsterStatComponent()
{
}

void UMonsterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMonsterStatComponent::InitStat()
{
	// 테이블과 행이 설정되어 있어야 한다.
	if (m_Table == nullptr || m_RowName.IsNone())
		return;

	// 모든 스탯을 다 지운다.
	m_Stats.Empty();

	FMonsterStatData* pMonsterStat = m_Table->FindRow<FMonsterStatData>(m_RowName, TEXT("MonsterStat"));

	// 데이터를 구성하고 있는 멤버들의 멤버 변수명 자체를 키값으로 해서 수치를 기록.
	InitStatFromStruct(FMonsterStatData::StaticStruct(), pMonsterStat);

	// 몬스터의 추가적인 런타임 스탯 추가

	// 부모 함수 호출
	Super::InitStat();
}
