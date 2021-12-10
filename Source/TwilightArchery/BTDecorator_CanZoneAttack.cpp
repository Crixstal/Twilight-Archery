// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanZoneAttack.h"
#include "BossCharacter.h"
#include "AIController.h"

UBTDecorator_CanZoneAttack::UBTDecorator_CanZoneAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Can Launch Zone Attack");
}

bool UBTDecorator_CanZoneAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	float distancePlayerEnemy = FVector::Dist(npc->GetActorLocation(), npc->target->GetActorLocation());

	if (npc->isAttacking && npc->zoneAttack)
		return true;

	if ((distancePlayerEnemy > 450.f && !npc->isAttacking))
	{
		npc->isChasing = true;
		return false;
	}
	else
	{
		if (npc->isChasing)
			return false;
		else
			return true;
	}
}