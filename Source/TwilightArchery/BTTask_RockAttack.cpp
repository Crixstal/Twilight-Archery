#include "BTTask_RockAttack.h"
#include "AIController.h"
#include "BossCharacter.h"

UBTTask_RockAttack::UBTTask_RockAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("RockAttack");
}

EBTNodeResult::Type UBTTask_RockAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	if (npc == nullptr)
		npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	if (!npc->isAttacking)
		npc->RockAttack();

	return EBTNodeResult::Succeeded;
}