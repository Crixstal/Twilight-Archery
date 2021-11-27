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

	arrowsCount = maxArrows;
}

// Called every frame
void UBowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (OnCharge())
	{
		if (timerCharge == maxChargeTime) return;

		UE_LOG(LogTemp, Warning, TEXT("CHARGING %f"), timerCharge);

		timerCharge += GetWorld()->GetDeltaSeconds();
		timerCharge = FMath::Clamp(timerCharge, 0.f, maxChargeTime);
	}
}

void UBowComponent::OnStartAiming()
{
	bIsAiming = true;
	bIsCharging = false;

	if (!bHasToDrawArrow)
		StartCharging();
}

void UBowComponent::CancelAim()
{
	bIsCharging = false;
}

void UBowComponent::OnEndAiming()
{
	bCanShoot = true;
	bIsAiming = false;
	bHasShoot = false;
}

void UBowComponent::StartCharging()
{
	timerCharge = 0.f;
	bIsCharging = true;
	bHasToDrawArrow = false;
}

float map(float value, float istart, float istop, float ostart, float ostop) 
{
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

void UBowComponent::Shoot(FVector ShootDirection, FTransform ShootTransform)
{
	bHasShoot = true;
	bIsCharging = false;
	bHasToDrawArrow = true;
	bCanShoot = false;

	float mult = map(timerCharge, 0.f, maxChargeTime, minChargeVelocityMultiplier, maxChargeVelocityMultiplier);
	FVector arrowVelocity = ShootDirection * mult;

	//UE_LOG(LogTemp, Warning, TEXT("Mult = %f"), timerCharge);

	AArrow* arrow = GetWorld()->SpawnActor<AArrow>(arrowBP, ShootTransform);
	arrow->Initialize(arrowVelocity);

	Reload();
}

void UBowComponent::Reload()
{
	if (arrowsCount == 0) return;

	arrowsCount -= 1;

	if (arrowsCount == 0)
		bNeedArrow = false;
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

bool UBowComponent::CanShoot()
{
	return bCanShoot && !bNeedArrow;
}