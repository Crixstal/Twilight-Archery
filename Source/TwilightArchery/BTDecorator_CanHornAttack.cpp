// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanHornAttack.h"
#include "BossCharacter.h"
#include "AIController.h"

UBTDecorator_CanHornAttack::UBTDecorator_CanHornAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Can Launch Zone Attack");
}

bool UBTDecorator_CanHornAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	if (npc->selectedAttack == 2)
		return true;
	else
		return false;
}
