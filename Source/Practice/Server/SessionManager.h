// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineSessionInterface.h"   // 세션
#include "Interfaces/OnlineFriendsInterface.h"   // 친구
#include "Interfaces/OnlineIdentityInterface.h"  // 로그인
#include "SessionManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionOperationComplete, bool, bWasSuccessful);

/**
 * 
 */
UCLASS()
class PRACTICE_API USessionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:
	IOnlineSessionPtr m_SessionInterface;

public:
	virtual void Initialize(FSubsystemCollectionBase& _Collection) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Session")
	void CreateSession(int32 _MaxPlayer);


	UFUNCTION(BlueprintCallable, Category = "Session")
	void FindSession();


	UFUNCTION(BlueprintCallable, Category = "Session")
	void JoinSession(int32 _Idx);

public:
	void OnCreateSessionCompleted(FName _SessionName, bool _bSuccess);
	void OnFindeSessionCompleted(bool _bSuccess);
	void OnJoinSessionCompleted(FName _SessionName, EOnJoinSessionCompleteResult::Type _Result);

protected:
	UPROPERTY(BlueprintAssignable)
	FOnSessionOperationComplete OnCreateSession_Blueprint;

protected:
	bool m_bSessionCreated;
	bool m_bSessionSearching;
};
