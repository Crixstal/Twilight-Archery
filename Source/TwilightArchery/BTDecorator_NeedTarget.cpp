// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_NeedTarget.h"
#include "BossCharacter.h"
#include "AIController.h"


UBTDecorator_NeedTarget::UBTDecorator_NeedTarget(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("NeedATarget");
}

bool UBTDecorator_NeedTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	ABossCharacter* npc = Cast<ABossCharacter>(actualEnemy->GetPawn());


	if (npc->haveATarget)
		return false;
	else
		return true;

}
