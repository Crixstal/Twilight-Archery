// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "GameFramework/Character.h"
#include "TwilightArcheryCharacter.h"
#include "BossCharacter.h"
#include "AIController.h"


UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("ChasePlayer");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	if (npc == nullptr)
		npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	ATwilightArcheryCharacter* me = Cast<ATwilightArcheryCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	

	actualEnemy->MoveToLocation(npc->target->GetActorLocation(), 1.f, false);

	return EBTNodeResult::Succeeded;
}