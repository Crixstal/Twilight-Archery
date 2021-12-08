#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CanRockAttack.generated.h"

UCLASS()
class TWILIGHTARCHERY_API UBTDecorator_CanRockAttack : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CanRockAttack(FObjectInitializer const& object_initializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
