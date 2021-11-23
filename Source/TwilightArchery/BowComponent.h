#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BowComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TWILIGHTARCHERY_API UBowComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBowComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Self\|Charge")
		float maxChargeTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Self\|Charge")
		float minChargeVelocityMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Self\|Charge")
		float maxChargeVelocityMultiplier = 1.f;

	UPROPERTY(Category = "Self\|Arrow", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AArrow> arrowBP;

protected:

	bool bIsCharging = false;
	bool bIsAiming = false;
	bool bHasShoot = false;

	float timerCharge = 0.f;

public:

	void OnStartAiming();
	void OnEndAiming();
	void Reload();
	void OnDrawArrow();
	void Shoot(FVector ShootDirection, FTransform ShootTransform);

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool OnCharge();
	UFUNCTION(BlueprintCallable)
	bool OnAim();
	UFUNCTION(BlueprintCallable)
	bool HasShoot();
};