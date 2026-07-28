// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "InputContainer.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "SkillDataBase.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Components/ShapeComponent.h"

#include "GenericTeamAgentInterface.h"

// 비동기 로딩 관련 헤더
#include "Engine/AssetManager.h"

// 키즈맷
#include "Kismet/GameplayStatics.h"

#include "NPC.h"

#include "../Item/Weapon/Weapon.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	// Skill Slot을 Editor 상에 보이도록 미리 추가해둔다
	for (int32 i = 0; i < (int32)ESkillSlot::COMMON_SECTION_END; ++i)
	{
		m_SkillSlots.Add(FSkillSlotInfo{ (ESkillSlot)i, });
	}

	// ...
}

// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// SkillComponent가 사용할 스킬들을 비동기 로딩 요청
	LoadSkill();

	m_SkeletalMeshCom = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	m_SelectedSkillSlot = &m_SkillSlots;
}

void USkillComponent::LoadSkill()
{
	// 1. 로드할 SkillDataAsset들의 경로를 모은다.
	TArray<FSoftObjectPath> AssetToLoad;

	// 슬롯에 장착되어 있는 각 스킬 에셋들의 경로를 모아준다.
	for (const FSkillSlotInfo& slotInfo : m_SkillSlots)
	{
		if (!slotInfo.SkillData.IsNull())
			AssetToLoad.Add(slotInfo.SkillData.ToSoftObjectPath());
	}

	// 2. 로딩할 에셋을 경로를 통해서 비동기 로딩 요청하기
	if (AssetToLoad.Num() > 0)
	{
		FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();

		AsyncLoadHandle = Streamable.RequestAsyncLoad(AssetToLoad,
			FStreamableDelegate::CreateUObject(this, &USkillComponent::OnSkillLoaded));
	}
}

void USkillComponent::OnSkillLoaded()
{
	m_bSkillLoaded = true;

	UE_LOG(LogTemp, Warning, TEXT("Skill Asset Load Complete!"));
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
 
void USkillComponent::UseSkill(int32 _SlotIndex, bool _IsBMode)
{
	if (_IsBMode)
		_SlotIndex -= ((uint32)ESkillSlot::COMMON_SECTION_END + 1);
	if (TryExecuteSkill(_SlotIndex))
	{
		// 방장(서버) 쪽에서 호출되는 상황인지 확인
		if (GetOwner()->HasAuthority())
		{
			// 멀티캐스트 - 다른 클라이언트들에게도 상황을 전달해야 한다.
			Multicast_SkillExecute(_SlotIndex, m_CurComboIdx);
		}
		else
		{
			// Client 환경
			// 서버에 스킬을 사용한 사실을 알려줘야 한다.
			Server_NotifySkillExecute(_SlotIndex, m_CurComboIdx);
		}
	}
}

bool USkillComponent::TryExecuteSkill(int32 _SlotIndex)
{
	if (!m_bSkillLoaded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Asset is not Loaded"));
		return false;
	}

	// 스킬 슬롯이 비어있는지 체크
	if (!IsValid((*m_SelectedSkillSlot)[_SlotIndex].SkillData.Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill Asset is not Equipped"));
		return false;
	}

	// 사용 중인 스킬이 없으면,
	if (m_CurSkillData == nullptr)
	{
		// 스킬을 사용할 수 있는 상태인지 체크
		if ((*m_SelectedSkillSlot)[_SlotIndex].SkillData->CanUseSkill(Cast<APawn>(GetOwner()), this))
		{
			// 콤보 초기화
			m_ComboWindow = false;
			m_CurComboIdx = 0;

			// 현재 사용중인 스킬로 등록
			m_CurSkillData = (*m_SelectedSkillSlot)[_SlotIndex].SkillData;
			// 스킬 실행
			m_CurSkillData->OnExecuteSkill(Cast<APawn>(GetOwner()), this);

			return true;
		}
		else
			return false;
	}
	// 이전에 사용한 스킬이 아직 안 끝났고, 새로 요청 들어온 스킬이 이전에 사용한 스킬과 동일하다.
	else if ((*m_SelectedSkillSlot)[_SlotIndex].SkillData == m_CurSkillData)
	{
		// 콤보 공격인지 체크
		if (m_CurSkillData->CanCombo && m_ComboWindow)
		{
			++m_CurComboIdx;
			FName NextSection = *FString::Printf(TEXT("Attack%d"), (m_CurComboIdx + 1));

			if (USkeletalMeshComponent* pMeshCom = GetOwner()->GetComponentByClass<USkeletalMeshComponent>())
			{
				if (pMeshCom->GetAnimInstance() != nullptr)
				{
					pMeshCom->GetAnimInstance()->Montage_JumpToSection(NextSection);
					m_ComboWindow = false;
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

void USkillComponent::TryExecuteSkillVisual(int32 _SlotIdx, int32 _ComboIdx)
{
	if (!m_SkillSlots.IsValidIndex(_SlotIdx) || !m_SkillSlots[_SlotIdx].SkillData.IsValid())
		return;

	// 최초 실행
	if (_ComboIdx == 0)
	{
		m_CurSkillData = m_SkillSlots[_SlotIdx].SkillData;
		m_CurSkillData->OnExecuteSkill(Cast<APawn>(GetOwner()), this);
	}
	else
	{
		m_CurComboIdx = _ComboIdx;
		FName NextSection = *FString::Printf(TEXT("Attack%d"), (m_CurComboIdx + 1));

		// 재생중인 몽타주의 섹션을 이동.
		if (m_SkeletalMeshCom && m_SkeletalMeshCom->GetAnimInstance())
		{
			m_SkeletalMeshCom->GetAnimInstance()->Montage_JumpToSection(NextSection);
		}
	}
}

void USkillComponent::EndSkill()
{
	// Animation 은 별개의 Thread라 낮은 확률로 m_CurSkillData가 nullptr일 수도 있다.
	if (m_CurSkillData != nullptr)
	{
		m_CurSkillData->OnEndSkill(Cast<APawn>(GetOwner()), this);
		m_CurSkillData = nullptr;
	}
	
	// 콤보 초기화
	m_ComboWindow = false;
	m_CurComboIdx = 0;

	// 스킬이 종료될 때 호출해달라고 등록받은 Delegate를 호출해준다.
	m_SkillEndDelegate.Broadcast(GetOwner());
}

void USkillComponent::CancleCurSkill()
{
	if (m_CurSkillData == nullptr)
		return;
	
	EndSkill();		// 원래 Montage Notify -> AnimInstance 에서 호출되지만, 강제 호출
}

bool USkillComponent::IsHit(AActor* _HitActor)
{
	if (_HitActor == nullptr)
		return false;

	IGenericTeamAgentInterface* pSkillUser = Cast<IGenericTeamAgentInterface>(GetOwner());
	IGenericTeamAgentInterface* pHit = Cast<IGenericTeamAgentInterface>(_HitActor);

	if (pSkillUser == nullptr || pHit == nullptr)
		return false;
	
	ETeamAttitude::Type type = pSkillUser->GetTeamAttitudeTowards(*_HitActor);

	return type == ETeamAttitude::Hostile;
}

USkillDataBase* USkillComponent::GetSkillData(ESkillSlot slotNum)
{
	return m_SkillSlots[(int32)slotNum].SkillData.Get();;
}

void USkillComponent::HitBoxOn()
{
	if (m_Weapon)
		m_PrevHitBoxSockPos = m_Weapon->GetHitSockPos();
	else
		m_PrevHitBoxSockPos = m_SkeletalMeshCom->GetSocketLocation(TEXT("HitBoxSock"));

	m_HitBoxOn = true;
}

void USkillComponent::HitBoxOff()
{
	m_HitBoxOn = false;

	m_HitActors.Empty();
}

void USkillComponent::HitBoxCheck()
{
	// 콜리전 Trace Channel을 활용해 직접 체크해준다.

	// 1. 현재 HitBoxSock의 위치를 알아야 한다.
	FVector CurSockPos;
	if (m_Weapon)
		CurSockPos = m_Weapon->GetHitSockPos();
	else
		CurSockPos = m_SkeletalMeshCom->GetSocketLocation(TEXT("HitBoxSock"));

	// 충돌 결과를 받아낼 배열
	TArray<FHitResult> HitResults;

	FCollisionQueryParams Params;			// 추가 사항
	Params.AddIgnoredActor(GetOwner());		// 소유자는 제외 (자기 공격 맞지 않도록)

	// 라인 충돌 검사 (Attack Trace 채널 사용)
/*	bool bHit = GetWorld()->LineTraceMultiByChannel(HitResults,
		m_PrevHitBoxSockPos,
		CurSockPos,
		ECC_GameTraceChannel4,
		Params);*/

	// Sphere 충돌 검사 (Attack Trace 채널 사용)
	FCollisionShape ColShape = FCollisionShape::MakeSphere(50.f);

	bool bHit = GetWorld()->SweepMultiByChannel(HitResults,
		m_PrevHitBoxSockPos,
		CurSockPos,
		FQuat::Identity,
		ECC_GameTraceChannel4,
		ColShape);

	// 한 명이라도 걸렸다
	if (bHit)
	{
		for (auto Result : HitResults)
		{
			if (!m_HitActors.Contains(Result.GetActor()))
			{
				m_HitActors.Add(Result.GetActor());

				// 데미지 주기. 적대관계 체크
				if (IsHit(Result.GetActor()))
				{
					APawn* SkillUser = Cast<APawn>(GetOwner());
					
					if (SkillUser->HasAuthority())
					{
						UGameplayStatics::ApplyDamage(Result.GetActor(),
							m_CurSkillData->Damage,
							SkillUser->GetController(),
							SkillUser,
							UDamageType::StaticClass());

						Multicast_PlayHitEffect(Result.ImpactPoint, Result.ImpactNormal);
					}
					else
					{
						// 서버에게 이펙트 발생 알리기 + 데미지 상황
						Server_NotifyDamage(Result.GetActor(), m_CurSkillData->Damage, Result.ImpactPoint, Result.ImpactNormal);
					}


					// 히트 이펙트
					if (m_CurSkillData->HitEffect)
					{
						// 충돌 위치에, m_HitEffect 나이아가라 재생하는 Actor 생성
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld()
							, m_CurSkillData->HitEffect
							, Result.ImpactPoint
							, Result.ImpactNormal.Rotation());
					}

					// 타격을 한 대상이 NPC 타입이면, 머리 위에 체력이 잠깐 보이게 한다.
					if (ANPC* pNPC = Cast<ANPC>(Result.GetActor()))
					{
						pNPC->ShowHPBar();
					}

					// 히트 했다!
					UE_LOG(LogTemp, Warning, TEXT("!! Hit !!"));
				}
			}
		}
	}

	// 디버그 렌더링 요청
	//DrawDebugLine(GetWorld(), CurSockPos, m_PrevHitBoxSockPos, FColor::Green, false, 1.f, 0);
	DrawDebugSphere(GetWorld(), CurSockPos, 30.f, 16, FColor::Green, false, 1.f, 0);

	m_PrevHitBoxSockPos = CurSockPos;
}

void USkillComponent::Fire()
{
	if (!m_CurSkillData || m_CurSkillData->SkillType != ESkillType::PROJECTILE)
		return;

	m_CurSkillData->OnFire(Cast<APawn>(GetOwner()), this);
}

void USkillComponent::Pitch()
{
	if (!m_CurSkillData || m_CurSkillData->SkillType != ESkillType::PROJECTILE)
		return;

	m_CurSkillData->OnPitch(Cast<APawn>(GetOwner()), m_TargetPawn, this);
}


void USkillComponent::Multicast_SkillExecute_Implementation(int32 _Slot, int32 _ComboIdx)
{
	APawn* pPawn = Cast<APawn>(GetOwner());

	// Multicast는 내 자신의 객체에서도 호출된다.
	// 내 Pawn은 이미 스킬을 실행했기 때문에, IsLocallyControlled 함수를 통해
	// 본인 자신일 경우엔 기능을 수행하지 않는다.
	if (pPawn && pPawn->IsLocallyControlled())
		return;

	// 똑같이 스킬을 따라한다.
	TryExecuteSkillVisual(_Slot, _ComboIdx);
}

void USkillComponent::Server_NotifySkillExecute_Implementation(int32 _Slot, int32 _ComboIdx)
{
	// 클라이언트가 호출하고 나서, 호스트의 서버의 해당 클라 객체 폰에 대한 동작을 정의한다.

	// 멀티캐스트
	Multicast_SkillExecute(_Slot, _ComboIdx);
}

bool USkillComponent::Server_NotifySkillExecute_Validate(int32 _Slot, int32 _ComboIdx)
{
	return true;
}

void USkillComponent::Multicast_PlayHitEffect_Implementation(FVector _ImpactPoint, FVector _ImpactNormal)
{
	if (APawn* pPawn = Cast<APawn>(GetOwner()))
	{
		if (pPawn->IsLocallyControlled() == false)
		{
			// 히트 이펙트
			if (m_CurSkillData->HitEffect)
			{
				// 충돌 위치에, m_HitEffect 나이아가라 재생하는 Actor 생성
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld()
					, m_CurSkillData->HitEffect
					, _ImpactPoint
					, _ImpactNormal.Rotation());
			}
		}
	}
}

void USkillComponent::Server_NotifyDamage_Implementation(AActor* _Target, float _Damage, FVector _ImpactPoint, FVector _ImpactNormal)
{
	APawn* SkillUser = Cast<APawn>(GetOwner());

	UGameplayStatics::ApplyDamage(_Target,
		_Damage,
		SkillUser->GetController(),
		SkillUser,
		UDamageType::StaticClass());

	Multicast_PlayHitEffect(_ImpactPoint, _ImpactNormal);
}

bool USkillComponent::Server_NotifyDamage_Validate(AActor* _Target, float _Damage, FVector _ImpactPoint, FVector _ImpactNormal)
{
	return true;
}