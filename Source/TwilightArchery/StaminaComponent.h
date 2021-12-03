#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TWILIGHTARCHERY_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStaminaComponent();

	virtual void BeginPlay() override;

	class ATwilightArcheryCharacter* player;

	void InitPlayer(ATwilightArcheryCharacter* inPlayer);

	FTimerHandle regenTimer;
	float regenDelay = 2.f;
	float deltaTime = 0.f;

	bool bShouldRegenPlayer = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SelfParameters")
	float currentStamina = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
	float maxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|DrainPercentage")
	float sprintDrain = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|DrainPercentage")
	float dodgeDrain = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|DrainPercentage")
	float jumpDrain = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|DrainPercentage")
	float aimDrain = 5.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters")
	bool bShouldDrain = false;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnJump();
	void OnStopJumping();

	void StartAiming();
	void StopAiming();

	void StartSprinting();
	void StopSprinting();

	void StartDodging();
	void StopDodging();

	void Regen();
	void Drain(float drainPercentage);
	void InstantDrain(float drainPercentage);
};