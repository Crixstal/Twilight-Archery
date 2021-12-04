// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_HornAttack.generated.h"

/**
 * 
 */
UCLASS()
class TWILIGHTARCHERY_API UBTTask_HornAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTTask_HornAttack(FObjectInitializer const& object_initializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	class ABossCharacter* npc;
};
