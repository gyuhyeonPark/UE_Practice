// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// 비동기 로딩 관련 헤더
#include "Engine/StreamableManager.h"

#include "SkillComponent.generated.h"

// 한 케릭터 당 5개의 스킬을 상정
UENUM(BlueprintType)
enum class ESkillSlot : uint8
{
	LeftClick UMETA(DisplayName = "LeftClick"),
	Skill_1 UMETA(DisplayName = "Skill1"),
	Skill_2 UMETA(DisplayName = "Skill2"),
	Skill_3 UMETA(DisplayName = "Skill3"),
	END UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FSkillSlotInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESkillSlot SlotType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class USkillDataBase> SkillData;	// 비동기 로드에 대해선 TSoftObjectPtr<>로 선언하기.
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillEnd, AActor*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICE_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

public:
	void Bind(UEnhancedInputComponent* _EIC, class UInputContainer* _InputContainer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 슬롯에 장착된 스킬들을 비동기 로딩한다.
	void LoadSkill();
	void OnSkillLoaded();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 키 입력 시 호출될 Delegate 함수
	// SkillSlot의 멤버를 param으로 받을 예정
	void UseSkill(int32 _SlotIndex);	// EIC Delegate Register
	bool TryExecuteSkill(int32 _SlotIndex);
	void TryExecuteSkillVisual(int32 _SlotIdx, int32 _ComboIdx);
	void EndSkill();

	void CancleCurSkill();

	// 스킬 쿨타임 등록 함수
	void AddSkillUseTime(FPrimaryAssetId _id)
	{
		m_mapSkillCoolTime.Add(_id, GetWorld()->GetTimeSeconds());
	}

	// 스킬 쿨타임 중인지 확인하는 함수
	bool IsSkillCoolTime(FPrimaryAssetId _id, float _MaxCoolTime)
	{
		if (!m_mapSkillCoolTime.Contains(_id))
			return false;

		// 현재시간 - 최근 사용시간 < 쿨타임
		if (GetWorld()->GetTimeSeconds() - m_mapSkillCoolTime[_id] < _MaxCoolTime)
			return true;

		return false;
	}

	// 스킬의 남은 쿨타임이 몇 초인지 반환하는 함수
	float GetSkillRemainCoolTime(FPrimaryAssetId _id, float _MaxCoolTime)
	{
		if (!m_mapSkillCoolTime.Contains(_id))
			return 0.f;

		// 최근 사용시간 + 스킬 쿨타임 시간
		return m_mapSkillCoolTime[_id] + _MaxCoolTime - GetWorld()->GetTimeSeconds();
	}

	// 정당한 피격인가?
	bool IsHit(AActor* _HitActor);

	class USkillDataBase* GetSkillData(ESkillSlot slotNum);
	const TSoftObjectPtr<class USkillDataBase> GetCurSkillInfo() { return m_CurSkillData; }


	void OpenComboWindow() { m_ComboWindow = true; }
	void CloseComboWindow() { m_ComboWindow = false; }

	USkeletalMeshComponent* GetMesh() { return m_SkeletalMeshCom; }

	virtual void HitBoxOn();
	void HitBoxOff();

	// 스킬 공격판정 활성화 시 매 프레임 마다 호출
	void HitBoxCheck();

	void Fire();

public:
	// 서버 입장에서 다른 클라이언트들에게 스킬 모션 재생을 알려주는 멀티캐스트 함수
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SkillExecute(int32 _Slot, int32 _ComboIdx);

	// 클라이언트 입장에서, 서버쪽에 자신이 스킬을 사용했음을 알리는 함수.
	// Server : 서버쪽에서 실행해야 하는 함수, WithValidation : 검증 포함
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_NotifySkillExecute(int32 _Slot, int32 _ComboIdx);

protected:
	// 스킬 장착 가능 슬롯 (Blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills", meta = (TitleProperty = "SlotType"))
	TArray<FSkillSlotInfo>	m_SkillSlots;
	
	// 지금 사용중인 스킬을 가리키는 포인터
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class USkillDataBase> m_CurSkillData;

	UPROPERTY()
	class USkeletalMeshComponent* m_SkeletalMeshCom;

	FVector m_PrevHitBoxSockPos;

	// 쿨타임 기록 컨테이너
	// 스킬의 고유 (경로 + 카테고리 명)을 키값으로 사용.
	// 사용한 시점의 월드 시간값을 저장 -> 매 프레임 += DT 필요 X
	TMap<FPrimaryAssetId, float> m_mapSkillCoolTime;

	// 비동기 로딩 담당자 핸들
	TSharedPtr<FStreamableHandle> AsyncLoadHandle;

	bool m_bSkillLoaded;

	bool m_ComboWindow;
	int32 m_CurComboIdx;

	TArray<UShapeComponent*> m_HitBoxes;
	// 중복 히트처리 방지
	TSet<TWeakObjectPtr<AActor>>	m_HitActors;

	// 공격 판정 활성화 여부
	bool m_HitBoxOn;

public:
	// 스킬 사용 후 종료 시 호출시켜줄 Delegate 들을 등록받을 수 있는 자료형
	FOnSkillEnd m_SkillEndDelegate;

};
