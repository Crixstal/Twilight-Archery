// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetTarget.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TwilightArcheryCharacter.h"
#include "AIController.h"
#include "BossCharacter.h"
#include <iostream>



UBTTask_GetTarget::UBTTask_GetTarget(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("GetTarget");
}

EBTNodeResult::Type UBTTask_GetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//TArray<AActor*> FoundPlayer;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATwilightArcheryCharacter::StaticClass(), FoundPlayer);
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());
	ATwilightArcheryCharacter* me = Cast<ATwilightArcheryCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());


	UWorld* world = GetWorld();

	TArray<AController*> Players;
	for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
	{
		AController* pl = Cast<AController>(*iter);
		if (!pl)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("PlayerNotValid")));
			return EBTNodeResult::Failed;
		}
		Players.Add(pl);
	}
	
	float minDist = -1;
	int incr = -1;

	if (Players.Num() > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("Player is %f %f %f"), me->GetActorLocation().X, me->GetActorLocation().Y, me->GetActorLocation().Z));
		for (int i = 0; i < Players.Num(); i++)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Possible target is %f %f %f"), Players[i]->GetPawn()->GetActorLocation().X, Players[i]->GetPawn()->GetActorLocation().Y, Players[i]->GetPawn()->GetActorLocation().Z));
			
			FVector savePos = Players[i]->GetPawn()->GetActorLocation();
			if (FVector::Dist(savePos, npc->GetActorLocation()) < minDist || minDist == -1)
			{
				minDist = FVector::Dist(savePos, npc->GetActorLocation());
				incr = i;
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::Printf(TEXT("min dist is %f"), minDist));
		npc->target = Players[incr]->GetPawn();
		npc->focustime = FMath::RandRange(npc->focusingTimeMin, npc->focusingTimeMax);
		npc->GetWorldTimerManager().SetTimer(npc->TimerHandleClock, npc, &ABossCharacter::Clock, 0.8f, true);
		npc->haveATarget = true;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Enemy Target is %f %f %f"), npc->target->GetActorLocation().X, npc->target->GetActorLocation().Y, npc->target->GetActorLocation().Z));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Enemy is focusing the target for %f sec"), npc->focustime));
	}

	return EBTNodeResult::Succeeded;
}
