// Copyright Epic Games, Inc. All Rights Reserved.

#include "TwilightArcheryGameMode.h"
#include "TwilightArcheryCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

ATwilightArcheryGameMode::ATwilightArcheryGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATwilightArcheryGameMode::BeginPlay()
{
	UGameplayStatics::CreatePlayer(GetWorld());
}
