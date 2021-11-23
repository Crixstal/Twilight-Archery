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

	player = Cast<ATwilightArcheryCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
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
}

void UStaminaComponent::SetupInputComponent(class UInputComponent* InputComponent)
{
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &UStaminaComponent::OnJump);
	InputComponent->BindAction("Jump", IE_Released, this, &UStaminaComponent::OnStopJumping);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &UStaminaComponent::StartSprinting);
	InputComponent->BindAction("Sprint", IE_Released, this, &UStaminaComponent::StopSprinting);

	InputComponent->BindAction("Aim", IE_Pressed, this, &UStaminaComponent::StartAiming);
	InputComponent->BindAction("Aim", IE_Released, this, &UStaminaComponent::StopAiming);

	InputComponent->BindAction("Dodge", IE_Pressed, this, &UStaminaComponent::StartDodging);
	InputComponent->BindAction("Dodge", IE_Released, this, &UStaminaComponent::StopDodging);
}

void UStaminaComponent::OnJump()
{
	player->Jump();
	bShouldDrain = true;
	InstantDrain(jumpDrain);
}

void UStaminaComponent::OnStopJumping()
{
	player->StopJumping();
	bShouldDrain = false;
	player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true, regenDelay);
}

void UStaminaComponent::StartAiming()
{
	player->bIsAiming = true;
	bShouldDrain = true;
	Drain(aimDrain);
}

void UStaminaComponent::StopAiming()
{
	player->bIsAiming = false;
	bShouldDrain = false;
	player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true, regenDelay);
}

void UStaminaComponent::StartSprinting()
{
	player->GetCharacterMovement()->MaxWalkSpeed = player->sprintWalkSpeed;

	if (player->GetVelocity().Size() != 0.f)
		player->bIsSprinting = true;
}

void UStaminaComponent::StopSprinting()
{
	player->GetCharacterMovement()->MaxWalkSpeed = player->baseWalkSpeed;
	player->bIsSprinting = false;
	bShouldDrain = false;
	player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true, regenDelay);
}

void UStaminaComponent::StartDodging()
{
	player->bIsDodging = true;
	bShouldDrain = true;
	Drain(dodgeDrain);
}

void UStaminaComponent::StopDodging()
{
	player->bIsDodging = false;
	bShouldDrain = false;
	player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true, regenDelay);
	Regen();
}

void UStaminaComponent::Regen()
{
	if (!bShouldDrain && !player->GetCharacterMovement()->IsFalling() && !player->bIsSprinting)
	{
		while (currentStamina != maxStamina)
		{
			float newStamina = currentStamina + (deltaTime * 20.f);
			currentStamina = FMath::Clamp(newStamina, 0.f, maxStamina);
			player->GetWorldTimerManager().SetTimer(regenTimer, this, &UStaminaComponent::Regen, deltaTime, true);
		}
	}
}

void UStaminaComponent::Drain(float drainPercentage)
{
	if (bShouldDrain)
	{
		float newStamina = currentStamina - (deltaTime * drainPercentage);
		currentStamina = FMath::Clamp(newStamina, 0.f, maxStamina);

		if (currentStamina == 0.f)
		{
			bShouldDrain = false;
			player->bIsSprinting = false;
			player->GetCharacterMovement()->MaxWalkSpeed = player->baseWalkSpeed;
			player->StopJumping();
		}

		else
			Drain(drainPercentage);
	}
}

void UStaminaComponent::InstantDrain(float drainPercentage)
{
	if (bShouldDrain && !player->GetCharacterMovement()->IsFalling())
	{
		float newStamina = currentStamina - (deltaTime * drainPercentage);
		currentStamina = FMath::Clamp(newStamina, 0.f, maxStamina);

		if (currentStamina == 0.f)
		{
			bShouldDrain = false;
			player->bIsSprinting = false;
			player->GetCharacterMovement()->MaxWalkSpeed = player->baseWalkSpeed;
			player->StopJumping();
		}

		else if (!player->bIsDodging)
			Drain(drainPercentage);
	}
}