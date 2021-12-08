#include "BTDecorator_CanRockAttack.h"
#include "BossCharacter.h"
#include "AIController.h"

UBTDecorator_CanRockAttack::UBTDecorator_CanRockAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Can Launch Rock Attack");
}

bool UBTDecorator_CanRockAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	if (npc->selectedAttack == 3)
		return true;
	else
		return false;
}
