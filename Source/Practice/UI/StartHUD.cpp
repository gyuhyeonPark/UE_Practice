// Fill out your copyright notice in the Description page of Project Settings.


#include "../UI/StartHUD.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UStartHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 버튼 클릭 시 호출될 멤버함수 델리게이트 바인딩
	StartBtn->OnClicked.AddDynamic(this, &UStartHUD::StartButtonClicked);
	QuitBtn->OnClicked.AddDynamic(this, &UStartHUD::QuitButtonClicked);

	// 버튼에 마우스가 올라가거나 벗어날 때 호출될 멤버함수 델리게이트 바인딩
	StartBtn->OnHovered.AddDynamic(this, &UStartHUD::StartButtonHovered);
	StartBtn->OnUnhovered.AddDynamic(this, &UStartHUD::StartButtonUnHovered);

	QuitBtn->OnHovered.AddDynamic(this, &UStartHUD::QuitButtonHovered);
	QuitBtn->OnUnhovered.AddDynamic(this, &UStartHUD::QuitButtonUnHovered);
}

void UStartHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStartHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UStartHUD::StartButtonClicked()
{
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("NewMap"));
}

void UStartHUD::StartButtonHovered()
{
	// BP에서 적용해놨던 애니메이션을 재생한다.
	if (StartBtnFadeInAnim == nullptr)
		return;

	PlayAnimation(StartBtnFadeInAnim);
}

void UStartHUD::StartButtonUnHovered()
{
	if (StartBtnFadeOutAnim == nullptr)
		return;

	PlayAnimation(StartBtnFadeOutAnim);
}

void UStartHUD::QuitButtonClicked()
{
/*	UKismetSystemLibrary::QuitGame(GetWorld(),
		GetWorld()->GetFirstPlayerController(),
		EQuitPreference::Quit, true);*/
}

void UStartHUD::QuitButtonHovered()
{
	if (QuitBtnFadeInAnim == nullptr)
		return;

	PlayAnimation(QuitBtnFadeInAnim);
}

void UStartHUD::QuitButtonUnHovered()
{
	if (QuitBtnFadeOutAnim == nullptr)
		return;

	PlayAnimation(QuitBtnFadeOutAnim);
}
