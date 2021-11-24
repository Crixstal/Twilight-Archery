// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BasicAttack.h"
#include "BossCharacter.h"
#include "AIController.h"

UBTTask_BasicAttack::UBTTask_BasicAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("BasicAttack");
}

EBTNodeResult::Type UBTTask_BasicAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	if (!npc->isAttacking)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("START ATTACK")));
		npc->BasicAttack();
	}

	return EBTNodeResult::Succeeded;
}
