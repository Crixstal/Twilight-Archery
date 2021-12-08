#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RockAttack.generated.h"


UCLASS()
class TWILIGHTARCHERY_API UBTTask_RockAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_RockAttack(FObjectInitializer const& object_initializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	class ABossCharacter* npc;
};
