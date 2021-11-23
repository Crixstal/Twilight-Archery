// Fill out your copyright notice in the Description page of Project Settings.


#include "BowComponent.h"
#include "Arrow.h"

// Sets default values for this component's properties
UBowComponent::UBowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UBowComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UBowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OnCharge())
	{
		if (timerCharge == maxChargeTime) return;

		//UE_LOG(LogTemp, Warning, TEXT("AimTime = %f"), onAimingTimer);

		timerCharge += GetWorld()->GetDeltaSeconds();
		timerCharge = FMath::Clamp(timerCharge, 0.f, maxChargeTime);
	}
}

void UBowComponent::OnStartAiming()
{
	bIsAiming = true;
}

void UBowComponent::OnEndAiming()
{
	bIsAiming = false;
	bHasShoot = false;
}

void UBowComponent::OnDrawArrow()
{
	bIsCharging = true;
	timerCharge = 0.f;
}

float map(float value, float istart, float istop, float ostart, float ostop) 
{
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void UBowComponent::Shoot(FVector ShootDirection, FTransform ShootTransform)
{
	bHasShoot = true;
	bIsCharging = false;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Shooting");

	float mult = map(timerCharge, 0.f, maxChargeTime, minChargeVelocityMultiplier, maxChargeVelocityMultiplier);
	FVector arrowVelocity = ShootDirection * mult;

	AArrow* arrow = GetWorld()->SpawnActor<AArrow>(arrowBP, ShootTransform);
	arrow->Initialize(arrowVelocity);
}

void UBowComponent::Reload()
{

}

bool UBowComponent::OnCharge()
{
	return bIsCharging;
}

bool UBowComponent::OnAim()
{
	return bIsAiming;
}

bool UBowComponent::HasShoot()
{
	return bHasShoot;
}