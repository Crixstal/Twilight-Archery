// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_NeedTarget.generated.h"

/**
 * 
 */
UCLASS()
class TWILIGHTARCHERY_API UBTDecorator_NeedTarget : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTDecorator_NeedTarget(FObjectInitializer const& object_initializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;


};
