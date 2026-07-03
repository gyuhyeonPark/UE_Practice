// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillDataBase.generated.h"

UENUM(BlueprintType)
enum class ESkillType : uint8
{
	MELEE,
	PROJECTILE,
	BUFF,

};

UCLASS()
class PRACTICE_API USkillDataBase : public UPrimaryDataAsset // 일반 DataAsset과 다르게, 비동기 로딩 지원. 
{
	GENERATED_BODY()
	
public:
	USkillDataBase();

public:
	// Character를 받지 않는 이유?
	// 몬스터의 경우 꼭 Character의 Capsule Collider, 물리 연산 등을
	// 사용하지 않을 수 있기 때문에...
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Logic")
	bool CanUseSkill(class APawn* _SkillUser, class USkillComponent* _SkillCom);	// 오버라이딩 용
	virtual bool CanUseSkill_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom);
	// Blueprint 상에서 부모의 함수 호출 시 위의 함수가 호출된다.
	// 똑같은 함수 시그니쳐, 함수 이름 뒤 _Implementation 붙이는 것이 규약.
	// BP에서 오버라이딩 했음 BP를 실행하고, 없다면 Implementation을 호출하는 것.

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Logic")
	void OnExecuteSkill(class APawn* _SkillUser, class USkillComponent* _SkillCom);	// 오버라이딩 용
	virtual void OnExecuteSkill_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Logic")
	void OnEndSkill(class APawn* _SkillUser, class USkillComponent* _SkillCom);	// 오버라이딩 용
	virtual void OnEndSkill_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Logic")
	void OnFire(class APawn* _SkillUser, class USkillComponent* _SkillCom);
	virtual void OnFire_Implementation(class APawn* _SkillUser, class USkillComponent* _SkillCom);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Skill Logic")
	void OnPitch(class APawn* _SkillUser, class APawn* _Target, class USkillComponent* _SkillCom);
	virtual void OnPitch_Implementation(class APawn* _SkillUser, class APawn* _Target, class USkillComponent* _SkillCom);

	// 에셋 매니저를 통한 식별자 등록
	// 에셋 매니저가 이 에셋을 SkillData라는 카테고리로 인식하게 한다.
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("SkillData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	ESkillType SkillType;	// 스킬 타입

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	FName SkillName;	// 스킬 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	FText DisplayName;	// 스킬 이름 (현지화 버전)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	FText Description;	// 스킬 설명 (현지화 버전)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillAnim")
	UAnimMontage* Montage;	// 스킬 모션

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillAnim")
	float MontageSpeed;		// 스킬 시전 시 캐릭터 회전 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Projectile")
	TSubclassOf<class AProjectileBase> ProjectileClass;		// 생성시킬 투사체의 UCLASS 정보를 가리킴
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|PitchProjectile")
	TSubclassOf<class APitchProjectile> PitchProjectileClass;		// 생성시킬 투사체의 UCLASS 정보를 가리킴

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Effect")
	class UNiagaraSystem* HitEffect;		// 생성시킬 투사체의 UCLASS 정보를 가리킴

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	float Damage; // 스킬 데미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	float CoolTime; // 스킬 재사용에 걸리는 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	float Cost;			// 스킬 사용 비용

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	float Range;		// 스킬 유효 사거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	bool CanMove;		// 이동 중 사용 가능 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	bool CanJump;		// 공중 사용 가능 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	bool CanCombo;		// 공중 사용 가능 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	float MoveSpeedScale;		// 스킬 시전 시 이동 속도 변화량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill|Info")
	float RotateSpeed;		// 스킬 시전 시 캐릭터 회전 속도


};
