// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_GetAttack.h"
#include "BossCharacter.h"
#include "AIController.h"

UBTDecorator_GetAttack::UBTDecorator_GetAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Can Launch Zone Attack");
}

bool UBTDecorator_GetAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	if (npc->chooseRdAtt == true)
		return true;
	else
		return false;
}
