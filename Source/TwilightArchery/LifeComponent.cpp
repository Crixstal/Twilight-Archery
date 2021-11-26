// Fill out your copyright notice in the Description page of Project Settings.

#include "LifeComponent.h"

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
}

// Called every frame
void ULifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULifeComponent::LifeDown(int value)
{
	if (currentLife > 0)
	{
		currentLife -= value;
		healthUpdate.Broadcast();

		if (currentLife <= 0)
			deathEvent.Broadcast();
	}
}

void ULifeComponent::LifeUp(int value)
{
	if (currentLife < maxLife)
	{
		currentLife += value;
		healthUpdate.Broadcast();
	}
}
