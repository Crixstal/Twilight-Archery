// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsRotating.h"
#include "BossCharacter.h"
#include "AIController.h"

UBTDecorator_IsRotating::UBTDecorator_IsRotating(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Is Rotating?");
}

bool UBTDecorator_IsRotating::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	return npc->isRotating;
}