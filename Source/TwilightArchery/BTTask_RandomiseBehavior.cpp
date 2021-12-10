// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RandomiseBehavior.h"
#include "BossAIController.h"
#include "BossCharacter.h"


UBTTask_RandomiseBehavior::UBTTask_RandomiseBehavior(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Randomiser");
	
}

EBTNodeResult::Type UBTTask_RandomiseBehavior::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	int numberOfScript = blackboard->GetValueAsInt(TEXT("NumberOfScript"));
	AAIController* actualEnemy = OwnerComp.GetAIOwner();
	if (npc == nullptr)
		npc = Cast<ABossCharacter>(actualEnemy->GetPawn());

	if (numberOfScript == 0)
		return EBTNodeResult::Failed;

	if (value == 0)
	{
		for (int i = 0; i < numberOfScript; i++)
		{
			int newValue = i + 1;
			values.push_back(newValue);
			values.push_back(newValue);
			values.push_back(newValue);
		}

		int incr = FMath::RandRange(1, numberOfScript * 3);
		value = values[incr - 1];
		values[value * 3 - 1] = 0;

	}
	else
	{
		prepreviousValue = previousValue;
		previousValue = value;
		int incr = FMath::RandRange(1, numberOfScript * 3);
		value = values[incr - 1];


		while (value == 0)
		{
			incr = FMath::RandRange(1, numberOfScript * 3);
			value = values[incr - 1];
		}

		if (value == previousValue)
			values[value * 3 - 2] = 0;
		else 
		{
			if (previousValue == 0)
			{
				values[prepreviousValue * 3 - 1] = prepreviousValue;
				values[prepreviousValue * 3 - 2] = prepreviousValue;
				values[prepreviousValue * 3 - 3] = prepreviousValue;
			}
			else
			{
				values[previousValue * 3 - 1] = previousValue;
				values[previousValue * 3 - 2] = previousValue;
				values[previousValue * 3 - 3] = previousValue;
			}
			
			values[value * 3 - 1] = 0;
		}
	}

	npc->selectedAttack = value;
	npc->chooseRdAtt = false;

	return EBTNodeResult::Succeeded;
}
