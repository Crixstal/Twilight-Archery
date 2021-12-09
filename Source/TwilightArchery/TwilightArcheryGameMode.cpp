// Copyright Epic Games, Inc. All Rights Reserved.

#include "TwilightArcheryGameMode.h"
#include "TwilightArcheryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/PlayerStart.h"

ATwilightArcheryGameMode::ATwilightArcheryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/TwilightArcheryCharacter_BP"));
	
	if (PlayerPawnBPClass.Class != NULL)
		DefaultPawnClass = PlayerPawnBPClass.Class;
}

void ATwilightArcheryGameMode::BeginPlay()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStarts);

	for (AActor* playerStart : playerStarts)
		if (playerStart->ActorHasTag(TEXT("P1")))
		{
			playerStarts.Remove(playerStart);
			break;
		}

	UGameplayStatics::CreatePlayer(GetWorld());

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), DefaultPawnClass, players);

	// Remove potential excess players
	int i = players.Num();
	while (i > 2)
	{
		AActor* toDelete = players.Last();
		toDelete->Destroy();
		players.Remove(toDelete);
		i = players.Num();
	}

	// Updates splitscreen
	auto gameViewport = GetWorld()->GetGameViewport();
	gameViewport->SetForceDisableSplitscreen(false);
	gameViewport->MaxSplitscreenPlayers = 2;
	gameViewport->UpdateActiveSplitscreenType();

	Super::BeginPlay();
}

AActor* ATwilightArcheryGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (playerStarts.Num() <= 0)
	{
		AActor* firstStart = Super::ChoosePlayerStart_Implementation(Player);
		firstStart->Tags.Add(FName("P1"));
		return firstStart;
	}
	
	int index = FMath::RandRange(0, playerStarts.Num() - 1);
	AActor* chosenStart = playerStarts[index];

	playerStarts.RemoveAt(index);
	return chosenStart;
}
