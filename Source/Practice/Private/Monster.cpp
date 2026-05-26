// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "../GlobalEnum.h"
#include "MonsterStatComponent.h"

// Sets default values
AMonster::AMonster()
{
	// 스탯 컴포넌트 추가
	m_NPCStatCom = m_StatCom = CreateDefaultSubobject<UMonsterStatComponent>(TEXT("StatComponent"));
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	// TeamId
	SetGenericTeamId((uint8)ETeamType::Enemy);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

