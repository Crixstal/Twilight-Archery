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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void LifeDown(int value = 1);
	void LifeUp(int value = 1);

	UPROPERTY(BlueprintAssignable, Category = "SelfParameters|Delegates")
		FDeathEvent deathEvent;
	UPROPERTY(BlueprintAssignable, Category = "SelfParameters|Delegates")
		FHealthUpdate healthUpdate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SelfParameters")
		int currentLife = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
		int maxLife = 100;
};
