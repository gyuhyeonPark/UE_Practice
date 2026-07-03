// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

#include "Net/UnrealNetwork.h"

#include "../UI/NPCBar.h"

#include "Components/WidgetComponent.h"

#include "NPC.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Tick마다 할 일이 없다.
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UWidgetComponent* WidgetCom = GetOwner()->GetComponentByClass<UWidgetComponent>())
	{
		if (UNPCBar* pWidget = Cast<UNPCBar>(WidgetCom->GetWidget()))
		{
			m_OnTakeDamage.AddDynamic(pWidget, &UNPCBar::UpdateHPBar);
		}
	}
	
}

void UStatComponent::OnRegister()
{
	Super::OnRegister();

	InitStat();
}

void UStatComponent::InitStat()
{
	float Value = GetStat("MaxHP");
	if (Value)
		AddStat(TEXT("CurHP"), Value);

	Value = GetStat("MaxMP");
	if (Value)
		AddStat(TEXT("CurMP"), Value);

	Value = GetStat("MaxSP");
	if (Value)
		AddStat(TEXT("CurSP"), Value);

	// 프리뷰 객체의 변경점을 UE Editor에게 알림. UI의 값을 객체의 값으로 재반영 강제하기.
	Modify();
}

void UStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 리플리케이트 하고 싶은 멤버를 등록
	DOREPLIFETIME(UStatComponent, m_Stats);
}

void UStatComponent::OnRep()
{
	if (m_OnTakeDamage.IsBound())
	{
		m_OnTakeDamage.Broadcast(this);
	}
}

void UStatComponent::InitStatFromStruct(UScriptStruct* _InStruct, const void* _StructPtr)
{
	if (_InStruct == nullptr || _StructPtr == nullptr)
		return;

	// 구조체 정보를 순회하면서 멤버 데이터를 확인한다.
	for (TFieldIterator<FProperty> Iter(_InStruct); Iter; ++Iter)
	{
		FProperty* Property = *Iter;

		// 멤버 변수 이름
		FName StatName = Property->GetFName();

		// Float 타입 멤버만 필터링
		FFloatProperty* FloatPro = CastField<FFloatProperty>(Property);

		if (FloatPro)
		{
			float Val = FloatPro->GetPropertyValue_InContainer(_StructPtr);
			AddStat(StatName, Val);
		}
	}
}

void UStatComponent::PostEditChangeProperty(FPropertyChangedEvent& _Event)
{
	Super::PostEditChangeProperty(_Event);

	InitStat();
}

void UStatComponent::AddStat(FName _StatName, float _Amount)
{
	// 이미 스탯이 있으면
	if (FStatInfo* pInfo = m_Stats.FindByKey(_StatName))
		return;

	FStatInfo info = {};

	info.StatName = _StatName;
	info.Value = _Amount;

	m_Stats.Add(info);
}

float UStatComponent::GetStat(FName _StatName)
{
	if (FStatInfo* pInfo = m_Stats.FindByKey(_StatName))
		return pInfo->Value;

	return 0.0f;
}

void UStatComponent::SetStat(FName _StatName, float _Value)
{
	if (FStatInfo* pInfo = m_Stats.FindByKey(_StatName))
	{
		pInfo->Value = _Value;

		// CurHP가 변경된 경우, HPBar를 3초간 보이게 한다.
		if (_StatName == TEXT("CurHP"))
		{
			if (m_OnTakeDamage.IsBound())
			{
				m_OnTakeDamage.Broadcast(this);
			}
		}
	}
}

