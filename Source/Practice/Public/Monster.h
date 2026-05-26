// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC.h"
#include "Monster.generated.h"

UCLASS()
class PRACTICE_API AMonster : public ANPC
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonster();

public:
	class UMonsterStatComponent* GetStatComponent() const { return m_StatCom; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (DisplayName = "StatComponent"))
	class UMonsterStatComponent* m_StatCom;
};
