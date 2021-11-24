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

	if ((distancePlayerEnemy <= 200 && !npc->isAttacking) || npc->isAttacking)
	{
		/*FVector FacingVector = {(npc->GetActorLocation().X + npc->target->GetActorLocation().X) / 2, (npc->GetActorLocation().X + npc->target->GetActorLocation().X) / 2,0};
		FRotator FacingRotator = FacingVector.Rotation();
		npc->SetActorRotation(FacingRotator, ETeleportType::None);*/
		return false;
	}
	else
		return true;
}
