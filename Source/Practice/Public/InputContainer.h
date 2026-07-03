// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputContainer.generated.h"

/**
 * 
 */
class UInputAction;
class UInputMappingContext;

UCLASS()
class PRACTICE_API UInputContainer : public UObject
{
	GENERATED_BODY()

public:
	// 데이터 비우기
	//void Clear() { m_Contexts.Empty(); m_mapIA.Empty(); }

	// 등록된 MappingContext 로부터 InputAction 뽑아내기
	void Init(AController* _Controller);

	// 등록된 InputAction 탐색 및 반환
	const UInputAction* FindIAByName(const FString& _Name);

	// 하나의 Mapping Context 에 연결된 InputAction 들을 찾아낸다.
	void AddInputAction(UInputMappingContext* _IMC);

	void SetBattingMode(AController* _Controller);
	void SetNormalMode(AController* _Controller);

protected:
/*	// 매핑 컨텍스트들을 저장
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TArray<UInputMappingContext*> m_Contexts;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* m_DefaultContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* m_BattingModeContext;

	// 매핑 컨텍스트랑 연결된 InputAction 들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TMap<FString, const UInputAction*> m_mapIA;
};
