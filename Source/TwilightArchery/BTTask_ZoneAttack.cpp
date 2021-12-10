// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ZoneAttack.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TwilightArcheryCharacter.h"
#include "AIController.h"
#include "BossCharacter.h"

UBTTask_ZoneAttack::UBTTask_ZoneAttack(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("ZoneAttack");
}

EBTNodeResult::Type UBTTask_ZoneAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	if (npc == nullptr)
		npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	if (!npc->isAttacking)
		npc->ZoneAttack();

	return EBTNodeResult::Succeeded;
}
