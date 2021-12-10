// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_HornAttack.h"
#include "AIController.h"
#include "BossCharacter.h"

UBTTask_HornAttack::UBTTask_HornAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("ZoneAttack");
}

EBTNodeResult::Type UBTTask_HornAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	if (npc == nullptr)
		npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	if (!npc->isAttacking)
		npc->HornAttack();

	return EBTNodeResult::Succeeded;
}
