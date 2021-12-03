#include "StaminaComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TwilightArcheryCharacter.h"

UStaminaComponent::UStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	currentStamina = maxStamina;
}

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UStaminaComponent::InitPlayer(ATwilightArcheryCharacter* inPlayer)
{
	if (inPlayer == nullptr)
		return;

	player = inPlayer;
}

void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	deltaTime = DeltaTime;

	if (player->bIsSprinting)
	{
		bShouldDrain = true;
		Drain(sprintDrain);
	}

	if (player->BowComponent->bIsAiming)
	{
		bShouldDrain = true;
		Drain(aimDrain);
	}
}

void UStaminaComponent::OnJump()
{
	bShouldDrain = true;
	InstantDrain(jumpDrain);
}

void UStaminaComponent::OnStopJumping()
{
	bShouldDrain = false;

	if (!player->bIsSprinting)
		player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true, regenDelay);
}

void UStaminaComponent::StartAiming()
{
	bShouldDrain = true;
	Drain(aimDrain);
}

void UStaminaComponent::StopAiming()
{
	bShouldDrain = false;
	player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true, regenDelay);
}

void UStaminaComponent::StartSprinting()
{
	bShouldDrain = true;
	Drain(sprintDrain);
}

void UStaminaComponent::StopSprinting()
{
	bShouldDrain = false;
	player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true, regenDelay);
}

void UStaminaComponent::StartDodging()
{
	bShouldDrain = true;
	InstantDrain(dodgeDrain);
}

void UStaminaComponent::StopDodging()
{
	bShouldDrain = false;

	if (!player->bIsDodging)
		player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true, regenDelay);
}

void UStaminaComponent::Regen()
{
	if (!bShouldDrain && !player->GetCharacterMovement()->IsFalling() && !player->bIsSprinting)
	{
		if (bShouldRegenPlayer)
		{
			bShouldRegenPlayer = false;
			player->OnStaminaRegen();
		}

		if (currentStamina != maxStamina)
		{
			float newStamina = currentStamina + (deltaTime * 20.f);
			currentStamina = FMath::Clamp(newStamina, 0.f, maxStamina);
		}
	}
}

void UStaminaComponent::Drain(float drainPercentage)
{
	if (bShouldDrain && !player->GetCharacterMovement()->IsFalling())
	{
		float newStamina = currentStamina - (deltaTime * drainPercentage);
		currentStamina = FMath::Clamp(newStamina, 0.f, maxStamina);

		if (currentStamina == 0.f)
		{
			bShouldDrain = false;
			bShouldRegenPlayer = true;
			player->OnStaminaEmpty();
			player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true, regenDelay);
		}
	}
}

void UStaminaComponent::InstantDrain(float drainPercentage)
{
	if (bShouldDrain && !player->GetCharacterMovement()->IsFalling())
	{
		float newStamina = currentStamina - drainPercentage;
		currentStamina = FMath::Clamp(newStamina, 0.f, maxStamina);

		if (currentStamina == 0.f)
		{
			bShouldDrain = false;
			bShouldRegenPlayer = true;
			player->OnStaminaEmpty();
		}
	}
}