// Copyright Epic Games, Inc. All Rights Reserved.

#include "TwilightArcheryCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Arrow.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// ATwilightArcheryCharacter

ATwilightArcheryCharacter::ATwilightArcheryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	BowMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
	BowMesh->SetupAttachment(GetMesh(), FName("BowSocket"));

	ArrowMesh2 = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	ArrowMesh2->SetupAttachment(BowMesh, FName("ArrowSocket"));

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ATwilightArcheryCharacter::BeginPlay()
{
	Super::BeginPlay();

	CameraBoom->TargetArmLength = baseArmLength;
	CameraBoom->SocketOffset = baseArmOffset;

	targetArmLength = baseArmLength;

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());

	ArrowMesh2->SetHiddenInGame(true);
}

void ATwilightArcheryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraBoom();

	if (bIsAiming)
	{
		FHitResult hitResult;
		FVector end = FollowCamera->GetComponentLocation() + (FollowCamera->GetComponentRotation().Vector() * 100000.f);
		bool traced = GetWorld()->LineTraceSingleByChannel(hitResult, FollowCamera->GetComponentLocation(), end, ECollisionChannel::ECC_Visibility);

		if (traced)
			DrawDebugSphere(GetWorld(), hitResult.Location, 15.f, 16, FColor::Red);
	}
}

void ATwilightArcheryCharacter::UpdateCameraBoom()
{
	if (timerArmCamera > 0.f)
	{
		float t = timerArmCamera / delayArmBaseToAim;
		t = bIsAiming ? 1.f - t : t;

		float curveValue = armCurve->GetFloatValue(t);

		float newLengthTarget = FMath::Lerp(baseArmLength, aimArmLength, curveValue);
		float newOffsetTargetY = FMath::Lerp(baseArmOffset.Y, aimArmOffset.Y, curveValue);


		CameraBoom->TargetArmLength = FMath::Clamp(newLengthTarget, aimArmLength, baseArmLength);
		//CameraBoom->TargetOffset newOffsetTarget;//FMath::Clamp(newOffsetTarget, aimArmOffset, baseArmOffset);
		CameraBoom->SocketOffset.Y = FMath::Clamp(newOffsetTargetY, baseArmOffset.Y, aimArmOffset.Y);

		UE_LOG(LogTemp, Warning, TEXT("Offset = %f"), CameraBoom->SocketOffset.Y);

		timerArmCamera -= GetWorld()->GetDeltaSeconds();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATwilightArcheryCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Toggle Splitscreen", IE_Pressed, this, &ATwilightArcheryCharacter::OnToggleSplitscreen);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATwilightArcheryCharacter::OnJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATwilightArcheryCharacter::OnStopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed,  this, &ATwilightArcheryCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATwilightArcheryCharacter::StopSprinting);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATwilightArcheryCharacter::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATwilightArcheryCharacter::StopAiming);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATwilightArcheryCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATwilightArcheryCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATwilightArcheryCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATwilightArcheryCharacter::LookUpAtRate);
}

void ATwilightArcheryCharacter::OnToggleSplitscreen()
{
	/*auto gameViewport = GetWorld()->GetGameViewport();

	gameViewport->MaxSplitscreenPlayers = 2;
	gameViewport->UpdateActiveSplitscreenType();

	if (gameViewport->IsSplitscreenForceDisabled())
	{
		GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, "Sscreen disabled");
	}*/
}

void ATwilightArcheryCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATwilightArcheryCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATwilightArcheryCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATwilightArcheryCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ATwilightArcheryCharacter::StartSprinting()
{
	if (bIsAiming) return;

	bIsSprinting = true;

	GetCharacterMovement()->MaxWalkSpeed = sprintWalkSpeed;
}

void ATwilightArcheryCharacter::StopSprinting()
{
	if (bIsAiming) return;

	bIsSprinting = false;

	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
}

void ATwilightArcheryCharacter::StartAiming()
{
	if (bIsSprinting)
		StopSprinting();

	bIsAiming = true;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = aimWalkSpeed;

	bUseControllerRotationRoll = true;
	bUseControllerRotationYaw = true;

	timerArmCamera = timerArmCamera > 0.f ? delayArmBaseToAim - timerArmCamera : delayArmBaseToAim;
	targetArmLength = aimArmLength;
}

void ATwilightArcheryCharacter::StopAiming()
{
	if (bReadyToShoot)
	{
		bHasShoot = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "Shooting");

		ArrowMesh2->SetHiddenInGame(true);

		//AArrow* arrow = GetWorld()->SpawnActor<AArrow>(arrowBP, ArrowMesh2->GetComponentTransform());

		return;
	}

	OnAimingEnd();
}

void ATwilightArcheryCharacter::OnAimingEnd()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;

	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	bReadyToShoot = false;
	bHasShoot = false;
	bIsAiming = false;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "StopAiming");

	timerArmCamera = timerArmCamera > 0.f ? delayArmBaseToAim - timerArmCamera : delayArmBaseToAim;
	targetArmLength = baseArmLength;

	bIsAiming = false;
}

void ATwilightArcheryCharacter::OnShootReady()
{
	bReadyToShoot = true;

	ArrowMesh2->SetHiddenInGame(false);

	/*FTransform transform;
	FActorSpawnParameters spawnParameters;

	AArrow* arrow = GetWorld()->SpawnActor<AArrow>(arrowBP);

	FAttachmentTransformRules attachmentRules(EAttachmentRule::KeepWorld, true);
	arrow->AttachToComponent(BowMesh, attachmentRules, FName("ArrowSocket"));*/
}

void ATwilightArcheryCharacter::OnJump()
{
	if (bIsAiming) return;

	Jump();
}

void ATwilightArcheryCharacter::OnStopJumping()
{
	if (bIsAiming) return;

	StopJumping();
}