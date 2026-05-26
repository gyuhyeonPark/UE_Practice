// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UMonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION()	// 블루 프린트에서 인식
		void AnimNotify_OnLandingStart();		// State 알림에 설정한 함수 구현 가능.

	// Montage에 등록된 Notify를 통해 만들 수 있는 함수다.
	UFUNCTION()
	void AnimNotify_SkillEnd();

	UFUNCTION()
	void AnimNotify_Fire();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float _DT) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Movement")
	float m_GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float m_MovementDir;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Movement")
	bool	m_IsFall;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Movement")
	float	m_VerticalSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Movement")
	float m_AccWorldTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCharacterMovementComponent* m_MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	class AMonster* m_Character;
};
