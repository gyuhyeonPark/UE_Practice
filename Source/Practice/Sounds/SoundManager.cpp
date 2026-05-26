// Fill out your copyright notice in the Description page of Project Settings.


#include "../Sounds/SoundManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void USoundManager::PlayBGM(USoundBase* _Sound, float _FadeTime)
{
	if (m_AudioCom && m_AudioCom->GetSound() == _Sound)
		return;

	// 먼저 재생 중인 BGM이 있었다
	if (m_AudioCom && m_AudioCom->IsPlaying())
	{
		// 재생 중이던 소리를 _FadeTime에 걸쳐 볼륨 0으로 만든다.
		m_AudioCom->FadeOut(_FadeTime, 0.f);
	}

	// 다음으로 재생시킬 소리가 없다면
	if (_Sound == nullptr)
		return;

	// 다음으로 재생시킬 소리가 있다면
	m_AudioCom = UGameplayStatics::CreateSound2D(GetWorld(), _Sound, 1.f, 1.f, 0.f, nullptr, false, true);
	
	if (m_AudioCom)
	{
		m_AudioCom->FadeIn(_FadeTime, 1.f);
	}
}

void USoundManager::StopBGM(float _FadeOutTime)
{
	if (m_AudioCom && m_AudioCom->IsPlaying())
		m_AudioCom->FadeOut(_FadeOutTime, 0.f);
}
