// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAIController.h"
#include "BossCharacter.h"

ABossAIController::ABossAIController(const FObjectInitializer& ObjectInitializer) : AAIController(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("/Game/Enemy/AI/BossBT.BossBT"));
	if (obj.Succeeded())
	{
		behaviorTree = obj.Object;
	}

	behaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboradComp"));

	SetActorTickEnabled(true);

}

void ABossAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(behaviorTree);
	behaviorComp->StartTree(*behaviorTree, EBTExecutionMode::Looped);
	ABossCharacter* Boss = Cast<ABossCharacter>(GetPawn());
	blackboard->SetValueAsObject(TEXT("SelfActor"), Boss);
	blackboard->SetValueAsInt(TEXT("NumberOfScript"), 2);
	blackboard->SetValueAsBool(TEXT("ChooseRandomAtt"), true);
	blackboard->SetValueAsBool(TEXT("IsAlive"), true);
}

void ABossAIController::OnDeath()
{
	blackboard->SetValueAsBool(TEXT("IsAlive"), false);
}