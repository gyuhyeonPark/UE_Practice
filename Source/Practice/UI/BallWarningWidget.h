// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BallWarningWidget.generated.h"

/**
 * Pitch Projectile이 만들어지면서 생성될 Widget.
 * Pitch Projectile에서 Delegate 함수를 BroadCast.
 */
UCLASS()
class PRACTICE_API UBallWarningWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	void SetWidgetPos(FVector _WorldDest);

protected:
	UPROPERTY(meta = (BindWidget))

	class UImage* m_WarningImg;
};
