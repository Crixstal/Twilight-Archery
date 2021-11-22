// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetTarget.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TwilightArcheryCharacter.h"
#include "AIController.h"
#include "BossCharacter.h"



UBTTask_GetTarget::UBTTask_GetTarget(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("GetTarget");
}

EBTNodeResult::Type UBTTask_GetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TArray<AActor*> FoundPlayer;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATwilightArcheryCharacter::StaticClass(), FoundPlayer);

	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	float minDist = -1;
	int incr;

	if (FoundPlayer.Num() > 0)
	{
		for (int i = 0; i >= FoundPlayer.Num(); i++)
		{
			FVector savePos = FoundPlayer[i]->GetActorLocation();
			if (FVector::Dist(savePos, npc->GetActorLocation()) < minDist || minDist == -1)
			{
				minDist = FVector::Dist(savePos, npc->GetActorLocation());
				incr = i;
			}
		}
	}
	

	if (incr != NULL)
	{
		npc->target = FoundPlayer[incr];
		npc->haveATarget = true;
	}

	return EBTNodeResult::Succeeded;
}
