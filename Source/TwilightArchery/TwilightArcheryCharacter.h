// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "BowComponent.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "TwilightArcheryCharacter.generated.h"

UCLASS(config=Game)
class ATwilightArcheryCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ATwilightArcheryCharacter();

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ArrowMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Aim\|Charge")
		float maxChargeTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Aim\|Charge")
		float minChargeVelocityMultiplier = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Aim\|Charge")
		float maxChargeVelocityMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bow")
	UBowComponent* BowComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera\|Rates")
	float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera\|Rates")
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera\|SpringParameters\|Delays")
	float delayArmBaseToAim = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera\|SpringParameters\|Curves")
	UCurveFloat* armCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera\|SpringParameters\|Base")
	float baseArmLength = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera\|SpringParameters\|Base")
	FVector baseArmOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera\|SpringParameters\|Aim")
	float aimArmLength = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera\|SpringParameters\|Aim")
	FVector aimArmOffset = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Booleans")
	bool bIsSprinting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Speeds")
	float baseWalkSpeed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Speeds")
	float sprintWalkSpeed = 550.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Speeds")
	float aimWalkSpeed = 300.f;

	UFUNCTION(BlueprintCallable)
	void OnAimingEnd();
	UFUNCTION(BlueprintCallable)
	void DrawArrow();

protected:

	UFUNCTION()
	void OnToggleSplitscreen();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	void StartSprinting();
	void StopSprinting();

	void StartAiming();
	void StopAiming();

	void OnJump();
	void OnStopJumping();

	void UpdateCameraBoom();

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	float timerArmCamera = 0.f;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

