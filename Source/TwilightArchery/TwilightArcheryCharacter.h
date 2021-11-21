// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Character.h"
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

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera\|Rates")
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Booleans")
	bool bIsAiming = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Booleans")
	bool bHasShoot = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Speeds")
	float baseWalkSpeed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Speeds")
	float sprintWalkSpeed = 550.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Speeds")
	float aimWalkSpeed = 300.f;

	UFUNCTION(BlueprintCallable)
	void OnAimingEnd();
	UFUNCTION(BlueprintCallable)
	void OnShootReady();


protected:
	
	bool bReadyToShoot = false;

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
	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	float timerArmCamera = 0.f;
	float targetArmLength = 0.f;

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

