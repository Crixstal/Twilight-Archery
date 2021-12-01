// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <vector>
#include "BTTask_RandomiseBehavior.generated.h"

/**
 * 
 */
UCLASS()
class TWILIGHTARCHERY_API UBTTask_RandomiseBehavior : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTTask_RandomiseBehavior(FObjectInitializer const& object_initializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	class ABossCharacter* npc;

	std::vector<int> values;
	int value = 0;
	int previousValue = -1;
	int prepreviousValue = -1;
};
