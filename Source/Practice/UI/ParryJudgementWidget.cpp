// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/ParryJudgementWidget.h"
#include "Components/Image.h"

void UParryJudgementWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (int32 i = 0; i < (int32)EParryJudgementType::END; ++i)
	{
		m_ImageSlot.Add(FJudgementImageInfo{ (EParryJudgementType)i, });
	}
}

void UParryJudgementWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UParryJudgementWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UParryJudgementWidget::Play(EParryJudgementType _Type)
{
	bool temp = false;
	if (m_ImageSlot[int32(_Type)].ImageData != nullptr)
	{
		temp = true;
		JudgementImg->SetBrushResourceObject(m_ImageSlot[int32(_Type)].ImageData.Get());
	}

	if (!temp)
	{
		const UEnum* EnumPtr = StaticEnum<EParryJudgementType>();

		if (EnumPtr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Warning : UI NOT REFRESHED, Type : %s"),
				*EnumPtr->GetNameStringByValue((int64)_Type));
		}
	}


	PlayAnimation(ImageFadeInOutAnim);
}
