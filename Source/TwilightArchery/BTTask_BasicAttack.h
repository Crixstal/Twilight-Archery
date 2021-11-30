// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class TWILIGHTARCHERY_API UBTTask_BasicAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	UBTTask_BasicAttack(FObjectInitializer const& object_initializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	class ABossCharacter* npc;

};
