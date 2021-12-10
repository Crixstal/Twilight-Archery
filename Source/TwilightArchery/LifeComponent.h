// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthUpdate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWILIGHTARCHERY_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class AActor* selfActor;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitActor(AActor* inActor);

	UFUNCTION(BlueprintCallable)
	void LifeDown(int value = 1);

	UFUNCTION(BlueprintCallable)
	void LifeUp(int value = 1);

	void SetInvincibility(bool value, float coolDown, bool toggle = false);

	UPROPERTY(BlueprintAssignable, Category = "SelfParameters|Delegates")
		FDeathEvent deathEvent;
	UPROPERTY(BlueprintAssignable, Category = "SelfParameters|Delegates")
		FHealthUpdate healthUpdate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SelfParameters")
		int currentLife = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
		int maxLife = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SelfParameters")
		float cooldownOnHit = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters|Booleans")
		bool bIsInvincible = false;

private:
	float invincibilityCooldown = 0.f;
	float timerInvincibility = 0.f;
	bool bInvincibilityToggle = false;

	void ResetInvincibility();
};
