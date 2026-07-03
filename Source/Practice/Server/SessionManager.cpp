// Fill out your copyright notice in the Description page of Project Settings.


#include "../Server/SessionManager.h"
#include "OnlineSessionSettings.h"

void USessionManager::Initialize(FSubsystemCollectionBase& _Collection)
{
	Super::Initialize(_Collection);

	// OSS 생성
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();

	if (OSS != nullptr)
	{
		m_SessionInterface = OSS->GetSessionInterface();

		m_SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
			FOnCreateSessionCompleteDelegate::CreateUObject(this,
				&USessionManager::OnCreateSessionCompleted));

		m_SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
			FOnFindSessionsCompleteDelegate::CreateUObject(this,
				&USessionManager::OnFindeSessionCompleted));

		m_SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
			FOnJoinSessionCompleteDelegate::CreateUObject(this,
				&USessionManager::OnJoinSessionCompleted));
	}
}

void USessionManager::CreateSession(int32 _MaxPlayer)
{
	if (m_bSessionCreated)
		return;

	// 만약에 이전에 생성한 세션이 남아있으면, 제거한다.
	if (m_SessionInterface->GetNamedSession(NAME_GameSession))
		m_SessionInterface->DestroySession(NAME_GameSession);

	FOnlineSessionSettings setting = {};
	
	setting.bIsLANMatch = true;						// LAN 환경
	setting.NumPublicConnections = _MaxPlayer;		// 최대 접속 가능 플레이어 수
	setting.bShouldAdvertise = true;				// 공개
	setting.bAllowJoinInProgress = true;			// 접속 가능

	// 방 생성 시도
	bool bResult = m_SessionInterface->CreateSession(0, NAME_GameSession, setting);

	if (bResult == false)
	{
		UE_LOG(LogTemp, Error, TEXT("!! CreateSession Failed !!"));
	}
	else
	{
		m_bSessionCreated = true;
	}
}

void USessionManager::OnCreateSessionCompleted(FName _SessionName, bool _bSuccess)
{
	OnCreateSession_Blueprint.Broadcast(_bSuccess);
}

void USessionManager::FindSession()
{
}

void USessionManager::OnFindeSessionCompleted(bool _bSuccess)
{
	m_bSessionSearching = false;

}

void USessionManager::JoinSession(int32 _Idx)
{
}

void USessionManager::OnJoinSessionCompleted(FName _SessionName, EOnJoinSessionCompleteResult::Type _Result)
{
}



