// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// 객체가 메모리에 등록될 때 호출(블루프린트를 열고 닫을 때, 프리뷰 객체가 생성되는 순간)
	virtual void OnRegister() override;

	virtual void InitStat();

public:
	void AddStat(FName _StatName, float _Amound) { m_Stats.Add(_StatName, _Amound); };
	float GetStat(FName _StatName)
	{
		if (float* pData = m_Stats.Find(_StatName))
		{
			return *pData;
		}
		else
			return 0.f;
	}
	void SetStat(FName _StatName, float _Value)
	{
		if (float* pData = m_Stats.Find(_StatName))
		{
			*pData = _Value;
		}
	}

protected:
	void InitStatFromStruct(UScriptStruct* _InStruct, const void* _StructPtr);

#if WITH_EDITOR
	// 에디터 모드에서만 사용 가능한 가상함수
	// 에디터 상에서 Preview 객체에 변경점이 발생하면 호출되는 함수
	virtual void PostEditChangeProperty(FPropertyChangedEvent& _Event) override;
#endif

protected:
	// 행 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FName m_RowName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TMap<FName, float> m_Stats;

};
