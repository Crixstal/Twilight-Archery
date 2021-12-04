// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwilightArcheryGameMode.generated.h"

UCLASS(minimalapi)
class ATwilightArcheryGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATwilightArcheryGameMode();
	virtual void BeginPlay() override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> playerStarts;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> players;
};



