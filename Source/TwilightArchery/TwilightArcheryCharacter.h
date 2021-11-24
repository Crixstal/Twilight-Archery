// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "BowComponent.h"
#include "Camera/CameraShake.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "TwilightArcheryCharacter.generated.h"

UCLASS(config=Game)
class ATwilightArcheryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATwilightArcheryCharacter();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Components", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Components", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

public:

	UPROPERTY(Category = "SelfParameters\|Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BowMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ArrowMesh2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Components")
	UBowComponent* BowComponent;



	// _______________________CAMERA PARAMETERS_____________________________
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Camera", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMatineeCameraShake> ShootShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Camera\|Rates")
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Camera\|Rates")
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Camera\|FOV")
	float baseCamFOV = 90.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Camera\|FOV")
	float aimCamFOV = 70.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Camera\|SpringParameters\|Curves")
	UCurveFloat* armCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Camera\|SpringParameters\|Delays")
	float delayArmBaseToAim = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Camera\|SpringParameters\|Base")
	float baseArmLength = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Camera\|SpringParameters\|Base")
	FVector baseArmOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Camera\|SpringParameters\|Aim")
	float aimArmLength = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Camera\|SpringParameters\|Aim")
	FVector aimArmOffset = FVector::ZeroVector;


	// _______________________SPEEDS PARAMETERS_____________________________
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|WalkSpeeds")
	float baseWalkSpeed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|WalkSpeeds")
	float sprintWalkSpeed = 550.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|WalkSpeeds")
	float aimWalkSpeed = 300.f;


	// _______________________OTHER PARAMETERS_____________________________
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Booleans")
		bool bIsSprinting = false;


	UFUNCTION(BlueprintCallable)
	void OnAimingEnd();
	UFUNCTION(BlueprintCallable)
	void DrawArrow();

private:

	UFUNCTION()
	void OnToggleSplitscreen();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void StartSprinting();
	void StopSprinting();

	void StartAiming();
	void StopAiming();

	void OnJump();
	void OnStopJumping();

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void UpdateCameraBoom();

	float timerArmCamera = 0.f;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	APlayerController* selfController;

public:

	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

