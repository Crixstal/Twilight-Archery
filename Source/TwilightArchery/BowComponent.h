#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BowComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartChargeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChargeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChargeMaxDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCancelChargeDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShootDelegate);


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

	UPROPERTY(BlueprintAssignable)
	FStartChargeDelegate startChargeDelegate;
	UPROPERTY(BlueprintAssignable)
	FChargeDelegate chargeDelegate;
	UPROPERTY(BlueprintAssignable)
	FChargeMaxDelegate chargeMaxDelegate;
	UPROPERTY(BlueprintAssignable)
	FCancelChargeDelegate cancelChargeDelegate;
	UPROPERTY(BlueprintAssignable)
	FShootDelegate shootDelegate;

	UPROPERTY(Category = "Self\|Arrow", EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AArrow> arrowBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Self\|Arrow")
		int maxArrows = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Self\|Arrow")
		bool bHasToDrawArrow = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Self\|Arrow")
		bool bCanShoot = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Self\|Arrow")
		bool bIsCharging = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Self\|Arrow")
		bool bIsMaxCharged = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Self\|Arrow")
		bool bIsAiming = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Self\|Arrow")
		bool bShouldAim = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Self\|Arrow")
		bool bHasShoot = false;

protected:

	bool bNeedArrow = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Self\|Charge")
	float timerCharge = 0.f;

	int arrowsCount = 0;

public:

	void OnStartAiming();
	void OnEndAiming();
	void CancelAim();
	void Reload();
	void StartCharging();
	void Shoot(class ATwilightArcheryCharacter* inPlayer, FVector ShootDirection, FTransform ShootTransform);
	
	float GetRatio();
	float GetRatio(float min, float max);

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetCurrentChargeTime();

	bool CanShoot();
	bool CanEndAiming();
};