// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeComponent.h"
#include "TwilightArcheryCharacter.h"

// Sets default values for this component's properties
ULifeComponent::ULifeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	currentLife = maxLife;
}

// Called when the game starts
void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Invincible : %d"), bIsInvincible);
}

// Called every frame
void ULifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UE_LOG(LogTemp, Warning, TEXT("Invincibility : %d"), bIsInvincible);

	if (bIsInvincible)
	{
		if (bInvincibilityToggle) return;

		if (timerInvincibility > 0.f)
		{
			timerInvincibility -= GetWorld()->GetDeltaSeconds();
			return;
		}

		ResetInvincibility();
	}
}

void ULifeComponent::InitActor(AActor* inActor)
{
	if (inActor == nullptr)
		return;

	selfActor = inActor;
}

void ULifeComponent::LifeDown(int value)
{
	if (bIsInvincible) return;

	if (currentLife > 0)
	{
		currentLife -= value;
		healthUpdate.Broadcast();

		if (currentLife <= 0)
		{
			deathEvent.Broadcast();
			return;
		}
	}
}

void ULifeComponent::LifeUp(int value)
{
	if (currentLife < maxLife)
	{
		currentLife += value;
		healthUpdate.Broadcast();
	}

	currentLife = FMath::Clamp(currentLife, 0, maxLife);
}

void ULifeComponent::ResetInvincibility()
{
	bIsInvincible = false;
	bInvincibilityToggle = false;
}

void ULifeComponent::SetInvincibility(bool value, float coolDown, bool toggle)
{
	if (!value)
	{
		ResetInvincibility();
		return;
	}

	bIsInvincible = true;

	if (toggle)
	{
		bInvincibilityToggle = true;

		return;
	}

	timerInvincibility = coolDown;
	bInvincibilityToggle = false;
}
