// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamage_Stat, class UStatComponent*, StatCom);

USTRUCT(BlueprintType)
struct FStatInfo
{
	GENERATED_BODY()

public:
	bool operator==(const FName& _Name) const
	{
		return StatName == _Name;
	}

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName StatName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;
};

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

	// 리플리케이트할 변수 등록
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 실제 리플리케이트가 발생했을 때 후속으로 호출되는 콜백 함수
	UFUNCTION()
	void OnRep();

protected:
	void InitStatFromStruct(UScriptStruct* _InStruct, const void* _StructPtr);

#if WITH_EDITOR
	// 에디터 모드에서만 사용 가능한 가상함수
	// 에디터 상에서 Preview 객체에 변경점이 발생하면 호출되는 함수
	virtual void PostEditChangeProperty(FPropertyChangedEvent& _Event) override;
#endif

public:
	void AddStat(FName _StatName, float _Amount);
	float GetStat(FName _StatName);
	void SetStat(FName _StatName, float _Value);

protected:
	// 행 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	FName m_RowName;

	UPROPERTY(ReplicatedUsing = OnRep, VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TArray<FStatInfo> m_Stats;

	FOnTakeDamage_Stat m_OnTakeDamage;
};
