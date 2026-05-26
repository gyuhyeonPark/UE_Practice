// Fill out your copyright notice in the Description page of Project Settings.


#include "StatComponent.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Tick마다 할 일이 없다.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
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

