// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetTarget.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TwilightArcheryCharacter.h"
#include "AIController.h"
#include "BossCharacter.h"
#include "LifeComponent.h"

#include <iostream>



UBTTask_GetTarget::UBTTask_GetTarget(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("GetTarget");
}

EBTNodeResult::Type UBTTask_GetTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	if (npc == nullptr)
		npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	/*float minDist = -1;
	int incr = -1;

	if (npc->Players.Num() > 0)
	{
		for (int i = 0; i < npc->Players.Num(); i++)
		{
			if (npc->Players[i]->GetPawn() == nullptr) continue;
			FVector savePos = npc->Players[i]->GetPawn()->GetActorLocation();
			if (FVector::Dist(savePos, npc->GetActorLocation()) < minDist || minDist == -1)
			{
				minDist = FVector::Dist(savePos, npc->GetActorLocation());
				incr = i;
			}
		}
		npc->target = npc->Players[incr]->GetPawn();
		npc->focustime = FMath::RandRange(npc->focusingTimeMin, npc->focusingTimeMax);
		npc->GetWorldTimerManager().SetTimer(npc->TimerHandleKFOT, npc, &ABossCharacter::KeepFocusOnTarget, 0.8f, true);
		npc->haveATarget = true;
	}*/
	if (onlyOnePlayerAlive)
	{
		npc->focustime = FMath::RandRange(npc->focusingTimeMin, npc->focusingTimeMax);
		npc->GetWorldTimerManager().SetTimer(npc->TimerHandleKFOT, npc, &ABossCharacter::KeepFocusOnTarget, 0.8f, true);
		npc->haveATarget = true;
		return EBTNodeResult::Succeeded;
	}


	if (npc->Players.Num() > 0)
	{
		if (npc->target == nullptr)
		{
				npc->target = npc->Players[FMath::RandRange(0, npc->Players.Num() - 1)]->GetPawn();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(89345, 5.f, FColor::Magenta, FString::Printf(TEXT("dps by target = %d"), npc->dpsFromTarget));
			GEngine->AddOnScreenDebugMessage(894345, 5.f, FColor::Magenta, FString::Printf(TEXT("dps by other player = %d"), npc->dpsFromTheOtherPlayer));
			if (npc->targetIsDead)
			{
				onlyOnePlayerAlive = true;
				for (int i = 0; i < npc->Players.Num(); i++)
					if (npc->target != npc->Players[i]->GetPawn())
						npc->target = npc->Players[i]->GetPawn();
			}

			if (npc->dpsFromTarget < npc->dpsFromTheOtherPlayer)
			{
				for (int i = 0; i < npc->Players.Num(); i++)
				{
					if (npc->target != npc->Players[i])
					{
						npc->target = npc->Players[i]->GetPawn();
						int saveValue = npc->dpsFromTarget;
						npc->dpsFromTarget = npc->dpsFromTheOtherPlayer;
						npc->dpsFromTheOtherPlayer = saveValue;

						npc->focustime = FMath::RandRange(npc->focusingTimeMin, npc->focusingTimeMax);
						npc->GetWorldTimerManager().SetTimer(npc->TimerHandleKFOT, npc, &ABossCharacter::KeepFocusOnTarget, 0.8f, true);
						npc->haveATarget = true;
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
		npc->focustime = FMath::RandRange(npc->focusingTimeMin, npc->focusingTimeMax);
		npc->GetWorldTimerManager().SetTimer(npc->TimerHandleKFOT, npc, &ABossCharacter::KeepFocusOnTarget, 0.8f, true);
		npc->haveATarget = true;
	}
	
	return EBTNodeResult::Succeeded;
}
