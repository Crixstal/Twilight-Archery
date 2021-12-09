#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "BowComponent.h"
#include "Camera/CameraShake.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TwilightArcheryCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHitEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FJumpEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDodgevent);

UCLASS(config=Game)
class ATwilightArcheryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATwilightArcheryCharacter();

	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FPauseEvent pauseEvent;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FHitEvent onHitEvent;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FJumpEvent jumpEvent;
	UPROPERTY(BlueprintAssignable, Category = "Delegate")
	FDodgevent dodgeEvent;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Components", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Components", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(Category = Actor, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* DodgeCapsule;

	void DebugLifeDown();
	void DebugLifeUp();

public:

	UPROPERTY(Category = "SelfParameters\|Components", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ArrowHandMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ArrowBowMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Components")
	UBowComponent* BowComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Components")
	class ULifeComponent* Life;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Character, meta = (AllowPrivateAccess = "true"))
	class UStaminaComponent* Stamina;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|WalkSpeeds")
	float dodgeSpeed = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|WalkSpeeds")
	float hitWalkSpeed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|WalkSpeeds")
	float lowStaminaWalkSpeed = 300.f;


	// _______________________OTHER PARAMETERS_____________________________
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Booleans")
		bool bIsSprinting = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Booleans")
		bool bIsDodging = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Booleans")
		bool bIsHit = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelfParameters\|Booleans")
		bool bIsJumping = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Booleans")
		bool bIsClimbing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Booleans")
		bool bIsDead = false;

	UFUNCTION(BlueprintCallable)
	void OnAimingEnd();
	UFUNCTION(BlueprintCallable)
	void DrawArrow();
	UFUNCTION(BlueprintCallable)
	void StopDodge();
	UFUNCTION(BlueprintCallable)
	void SetInvincible(bool value);
	UFUNCTION(BlueprintCallable)
	void TakeArrowBack();
	UFUNCTION(BlueprintCallable)
	void PlaceArrowOnBow();
	UFUNCTION(BlueprintCallable)
	void OnHit(const FVector& Normal);
	UFUNCTION(BlueprintCallable)
	void OnEndHit();

	void OnStaminaRegen();
	void OnStaminaEmpty();

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

	void StartDodge();
	void PauseGame();

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void UpdateCameraBoom();

	void SetLastControlDirection();

	float timerArmCamera = 0.f;

	FVector rightInputDir = FVector::ZeroVector;
	FVector forwardInputDir = FVector::ZeroVector;
	FVector lastControlDirection;
	//FVector inputDirection = FVector::ZeroVector;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector hitDirection = FVector::ZeroVector;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	bool CanSprint();
	bool CanDodge();
	bool CanJump();
	bool CanAim();

	APlayerController* selfController;

public:

	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};

