// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Task_UseSkill.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_API UTask_UseSkill : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTask_UseSkill();

public:
	// Task란 BT에서 어떤 상태인지 결정된 후 실제로 행하게 될 행동을 의미한다.

	// 행동 시작 시 최초로 호출된다.
	// param 1 : 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;



	// 행동 시 매 프레임 호출된다.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// MoveTo 같은 함수의 예시)
	// ExecuteTask에서 Target을 설정,
	// TickTask에서 Target을 계속 따라간다...

	// UBTTaskNode를 상속받은 클래스를 만들어 나만의 Task를 정의해보자!

public:
	void OnSkillEnd(AActor* _SkillUser, UBehaviorTreeComponent* _BTCom);

	// Tick에서 사용할 float(4Byte) 크기의 메모리 공간을 할당해준다.
	virtual uint16 GetInstanceMemorySize() const override
	{
		return sizeof(float);
	}

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector m_Target;

};

// UBehaviorTreeComponent의 의미?
// BT : 메모리에 하나만 존재한다.
// 해당 BT를 공유하는 몬스터는 수백일 수 있다.
// 해당 몬스터는 AIController를 갖고 있고,
// AIController에는 BT Component, BlackBoard가 있다.
// BT Component : BT를 지정, 사용하는 컴포넌트

// 때문에 BT에 존재하는 Task의 경우에도 메모리에 하나만 존재,
// 나를 사용하는 BT Component가 누군지 알아야 행동을 시켜줄 수 있다.

// 또한, Task 클래스 내부에 멤버변수를 설정할 땐 주의해야 한다.
// 해당 변수는 이를 참조하는 모든 BT Component 객체에 의해 공유되기 때문.
// 이를 용이하게 해주는 것이 NodeMemory.