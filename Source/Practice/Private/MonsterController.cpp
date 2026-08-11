// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController.h"
#include "Monster.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISenseConfig_Sight.h"
#include "GenericTeamAgentInterface.h"

// BehaviorTree / Blackboard
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"

// Global
#include "../GlobalEnum.h"

// StatComponent
#include "StatComponent.h"

// Path
#include "Navigation/PathFollowingComponent.h"

AMonsterController::AMonsterController()
{
	m_PerceptionCom = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionCom"));
	SetPerceptionComponent(*m_PerceptionCom);

	m_SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	if (m_SightConfig)
	{
		m_SightConfig->SightRadius = 3000.f; // AI 가 대상을 처음 감지할 수 있는 거리
		m_SightConfig->LoseSightRadius = 3500.f; // AI 가 대상을 처음 감지할 수 있는 거리
		m_SightConfig->PeripheralVisionAngleDegrees = 60.f; // 시전 정면방향을 기준으로, 반경 각도, 최대시야각은 x2 
		m_SightConfig->DetectionByAffiliation.bDetectEnemies = true; // 감지대상이 적대관계인경우 탐지한것으로 인정
		m_SightConfig->DetectionByAffiliation.bDetectFriendlies = false; // 감지대상이 우호관계인경우 탐지한것으로 인정
		m_SightConfig->DetectionByAffiliation.bDetectNeutrals = false; // 감지대상이 중립관계인경우 탐지한것으로 인정

		m_PerceptionCom->ConfigureSense(*m_SightConfig); // 인지 컴포넌트에 시각정보 추가
		m_PerceptionCom->SetDominantSense(m_SightConfig->GetSenseImplementation()); // 시각정보를 최우선 감각으로 사용할 것
	}

	// Damage 감각 정보 설정
	m_DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DanageConfig"));
	m_PerceptionCom->ConfigureSense(*m_DamageConfig); // 인지 컴포넌트에 데미지 정보 추가

	// 탐지 발생 시 호출될 Delegate 등록
	m_PerceptionCom->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterController::OnTargetDetected);

	// Pawn의 회전을 Controller도 따라가라
	//bSetControlRotationFromPawnOrientation = false;
}

void AMonsterController::OnTargetDetected(AActor* _Target, FAIStimulus _Stimulus)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Detect Enemy"));

	// 자극의 근원지를 추적할 수 없음.
	if (_Target == nullptr)
		return;

	// 빙의한 몬스터의 적대관계 기준으로, 인지한 대상이 적인지 아닌지 판단한다.
	AMonster* pMonster = Cast<AMonster>(GetPawn());
	if (pMonster == nullptr)
		return;

	if (pMonster == _Target)
		return;

	ETeamAttitude::Type type = pMonster->GetTeamAttitudeTowards(*_Target);

	// 적대관계일 시
	if (type == ETeamAttitude::Hostile)
	{
		// 이전에 감지대상으로 등록한 적이 있는 대상인지 확인
		FSensedTargetInfo* pInfo = FindSensedTarget(_Target);

		// 이전에 감지한 적이 없을 경우
		if (pInfo == nullptr)
		{
			// 없다면 감지 목록에 추가
			pInfo = &AddSensedTarget(_Target);
		}

		// 감지 여부 기록
		pInfo->bSensed = _Stimulus.WasSuccessfullySensed();

		// 인지 범위에서 벗어난 경우
		if (pInfo->bSensed == false)
		{
			// 이 때의 위치(놓친 시점의 위치)를 기록
			pInfo->LosePos = _Stimulus.StimulusLocation;

			// 인지를 놓쳤을 때의 시간값 기록
			pInfo->LoseTime = GetWorld()->GetTimeSeconds();
		}

		// 인지 정보가 시각 정보인지, 데미지로 인해 발생한 정보인지 구별하기
		static FAISenseID SightID = UAISense::GetSenseID<UAISense_Sight>();
		static FAISenseID DmgID = UAISense::GetSenseID<UAISense_Damage>();

		if (_Stimulus.Type == SightID)
		{
			if (pInfo->bSensed)
				pInfo->AggroValue += 10.f;
		}
		else if (_Stimulus.Type == DmgID)
		{
			pInfo->AggroValue += 20.f;
		}
	}
}

FSensedTargetInfo& AMonsterController::AddSensedTarget(AActor* _Target)
{
	FSensedTargetInfo info;
	info.Target = _Target;
	return m_SensedTargets.Add_GetRef(info);
}

FSensedTargetInfo* AMonsterController::FindSensedTarget(const AActor* _Target)
{
	for (uint8 i = 0; i < m_SensedTargets.Num(); ++i)
	{
		if (m_SensedTargets[i].Target == _Target)
			return &m_SensedTargets[i];
	}

	return nullptr;
}

/// <summary>
/// 
/// </summary>
/// <param name="_LimintTime">
/// 해당 시간 만큼 감지가 안되면 감지 리스트에서 제외
/// </param>
void AMonsterController::ExpireSensedTarget(float _LimintTime)
{
	float CurTime = GetWorld()->GetTimeSeconds();

	for (auto iter = m_SensedTargets.CreateIterator(); iter; ++iter)
	{
		// 1. 감지된 대상이 삭제된 경우 
		bool bRemove = false;
		
		if (iter->Target.IsValid() == false)
		{
			bRemove = true;
		}
		// 2. 인지를 놓친지 _LimitTime을 넘어선 경우
		else if (iter->bSensed == false)
		{
			if (_LimintTime < CurTime - iter->LoseTime)
			{
				bRemove = true;
			}
		}

		if (bRemove)
		{
			// iter가 가리키는 대상 삭제, 하나 이전 반복자을 가리킨다. -> ++iter 사용 가능
			iter.RemoveCurrent();
		}

	}
}


// 빙의 되면서 호출되는 함수.
void AMonsterController::OnPossess(APawn* _Pawn)
{
	Super::OnPossess(_Pawn);
	const IGenericTeamAgentInterface* pPawnTeam = Cast<IGenericTeamAgentInterface>(_Pawn);
	
	// Controller의 팀을 빙의한 _Pawn의 팀으로 동기화한다.
	if (pPawnTeam)
		SetGenericTeamId(pPawnTeam->GetGenericTeamId());
	else
		SetGenericTeamId((uint8)ETeamType::None);

	// 빙의할 대상(몬스터)의 스탯 컴포넌트를 가져온다.
	UStatComponent* pStatCom = _Pawn->FindComponentByClass<UStatComponent>();

	// 시야 인지범위 설정
	m_SightConfig->SightRadius = pStatCom->GetStat(TEXT("DetectRange"));
	m_SightConfig->LoseSightRadius = pStatCom->GetStat(TEXT("LoseDetectRange"));

	// 갱신, 위에서 정보를 config 갱신 시 아래와 같이 재설정 필요.
	m_PerceptionCom->ConfigureSense(*m_SightConfig);
	m_PerceptionCom->ConfigureSense(*m_DamageConfig);
	m_PerceptionCom->RequestStimuliListenerUpdate();

	if (m_BTAsset && m_BBAsset)
	{
		UBlackboardComponent* pBBCom = Blackboard;

		// 블랙보드 에셋을 전달하면, 그걸 사용할 컴포넌트 만들고, 전달한 블랙보드 에셋을 사용하도록 세팅한다.
		if (UseBlackboard(m_BBAsset, pBBCom))
		{
			// AIController에게 생성된 블랙보드 컴포넌트 주소를 알려준다.
			Blackboard = pBBCom;

			RunBehaviorTree(m_BTAsset);
		}
	}
}
