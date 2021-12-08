// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanBasicAttack.h"
#include "BossCharacter.h"
#include "AIController.h"

UBTDecorator_CanBasicAttack::UBTDecorator_CanBasicAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Can Launch Zone Attack");
}

bool UBTDecorator_CanBasicAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	if (npc->selectedAttack == 1)
		return true;
	else
		return false;
}
