// Fill out your copyright notice in the Description page of Project Settings.


#include "../Actor/PoseCopy.h"
#include "Components/PoseableMeshComponent.h"

// Sets default values
APoseCopy::APoseCopy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 특정 애니메이션의 동작을 캡쳐해서 따라하는 컴포넌트
	m_MeshCom = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("PoseableMeshComponent"));
	RootComponent = m_MeshCom;
} 

// Called when the game starts or when spawned
void APoseCopy::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(m_Duration);
}

// Called every frame
void APoseCopy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	m_Elapsed += DeltaTime;
}

void APoseCopy::CopyPos(USkeletalMeshComponent* _Com, class UMaterialInterface* _UseMtrl, float _Duration)
{
	if (!_Com)
		return;

	// 원본과 동일한 메쉬를 사용
	m_MeshCom->SetSkeletalMesh(_Com->GetSkeletalMeshAsset());

	// 원본의 현재 애니메이션 행렬 정보를 복사받는다.
	m_MeshCom->CopyPoseFromSkeletalComponent(_Com);

	// 재질을 자신에게 적용시킨다.
	// SkeletalMeshComponent의 부위별 Mesh에 전부 _UseMtrl을 적용한다.
	int32 MtrlCnt = m_MeshCom->GetNumMaterials();

	for (int32 i = 0; i < MtrlCnt; ++i)
	{
		m_MeshCom->SetMaterial(i, _UseMtrl);
	}

	m_Duration = _Duration;
}

