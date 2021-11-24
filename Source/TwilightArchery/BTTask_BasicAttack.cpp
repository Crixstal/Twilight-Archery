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

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CAN ATTACK")));
	if (!npc->isAttacking)
	{
		npc->Attack();
	}

	return EBTNodeResult::Succeeded;
}
