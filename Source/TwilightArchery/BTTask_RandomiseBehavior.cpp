// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RandomiseBehavior.h"
#include "BossAIController.h"

UBTTask_RandomiseBehavior::UBTTask_RandomiseBehavior(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Randomiser");
	
}

EBTNodeResult::Type UBTTask_RandomiseBehavior::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	int numberOfScript = blackboard->GetValueAsInt(TEXT("NumberOfScript"));

	if (numberOfScript == 0)
		return EBTNodeResult::Failed;

	if (value == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Value : %d"), numberOfScript);\

		for (int i = 0; i < numberOfScript; i++)
		{
			int newValue = i + 1;
			values.push_back(newValue);
			values.push_back(newValue);
			values.push_back(newValue);
		}

		int incr = FMath::RandRange(1, numberOfScript * 3);
		value = values[incr - 1];
	}
	else
	{
		int incr = FMath::RandRange(1, numberOfScript * 3);
		value = values[incr - 1];

		if (previousValue == 0)
		{
			values[value * 3 - 1] = 0;
			previousValue = value;
		}
		else
		{
			if (value == previousValue)
				values[value * 3 - 2] = 0;
			else
			{
				values[value * 3 - 1] = 0;
				values[previousValue * 3 - 1] = previousValue;
				values[previousValue * 3 - 2] = previousValue;
				previousValue = value;
			}
		}
	}
	
		
	

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("%d"), value));
	blackboard->SetValueAsInt(TEXT("SelectedAttack"), value);

	return EBTNodeResult::Succeeded;
}
