// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BossAIController.generated.h"



UCLASS()
class TWILIGHTARCHERY_API ABossAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABossAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

private:

	//COMPONENTS || BEHAVIORS
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BehaviorTree", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* behaviorComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "BehaviorTree", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* behaviorTree;

	class UBlackboardComponent* blackboard;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void OnDeath();

};
