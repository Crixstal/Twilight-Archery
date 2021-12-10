// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsNearPlayer.h"
#include "BossCharacter.h"
#include "AIController.h"


UBTDecorator_IsNearPlayer::UBTDecorator_IsNearPlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("NeedATarget");
}

bool UBTDecorator_IsNearPlayer::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	float distancePlayerEnemy = FVector::Dist(npc->GetActorLocation(), npc->target->GetActorLocation());
	
	if ((distancePlayerEnemy <= 500 && !npc->isAttacking) || npc->isAttacking )
		return false;
	else
		return true;
}
