#include "TwilightArcheryCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "LifeComponent.h"
#include "Arrow.h"
#include "GameFramework/SpringArmComponent.h"
#include "StaminaComponent.h"
#include "Kismet/GameplayStatics.h"

#define ARROW_SOCKET FName("ArrowSocket")

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

	ArrowHandMesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowHandMesh"));
	ArrowHandMesh->SetupAttachment(GetMesh(), FName("ArrowSocket"));

	ArrowBowMesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowBowMesh"));
	ArrowBowMesh->SetupAttachment(BowMesh, FName("ArrowSocket"));

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	DodgeCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DodgeCapsule"));
	DodgeCapsule->SetupAttachment(GetMesh());

	BowComponent = CreateDefaultSubobject<UBowComponent>("Bow Component");
	Stamina = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));
	Life = CreateDefaultSubobject<ULifeComponent>(TEXT("LifeComponent"));
}

void ATwilightArcheryCharacter::BeginPlay()
{
	Super::BeginPlay();

	CameraBoom->TargetArmLength = baseArmLength;
	CameraBoom->SocketOffset = baseArmOffset;

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());

	ArrowHandMesh->SetHiddenInGame(true);
	ArrowBowMesh->SetHiddenInGame(true);

	selfController = Cast<APlayerController>(GetController());

	DodgeCapsule->SetCapsuleHalfHeight(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	DodgeCapsule->SetCapsuleRadius(GetCapsuleComponent()->GetUnscaledCapsuleRadius());

	Stamina->InitPlayer(this);
}

void ATwilightArcheryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraBoom();

	if (bIsDodging)
	{
		if (GetCharacterMovement()->IsFalling())
		{
			StopDodge();
			return;
		}
		//FVector newPos = GetActorLocation() + lastControlDirection * dodgeSpeed * GetWorld()->GetDeltaSeconds();
		//SetActorLocation(newPos);
		AddMovementInput(lastControlDirection, 1.f);
	}
}

void ATwilightArcheryCharacter::UpdateCameraBoom()
{
	if (timerArmCamera > 0.f)
	{
		float t = timerArmCamera / delayArmBaseToAim;
		t = BowComponent->OnAim() ? 1.f - t : t;

		float curveValue = armCurve->GetFloatValue(t);

		float newLengthTarget = FMath::Lerp(baseArmLength, aimArmLength, curveValue);
		float newOffsetTargetY = FMath::Lerp(baseArmOffset.Y, aimArmOffset.Y, curveValue);
		float newFOV = FMath::Lerp(baseCamFOV, aimCamFOV, curveValue);

		CameraBoom->TargetArmLength = FMath::Clamp(newLengthTarget, aimArmLength, baseArmLength);
		CameraBoom->SocketOffset.Y = FMath::Clamp(newOffsetTargetY, baseArmOffset.Y, aimArmOffset.Y);

		FollowCamera->FieldOfView = FMath::Clamp(newFOV, aimCamFOV, baseCamFOV);

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

	PlayerInputComponent->BindAction("DebugLifeDown", IE_Pressed, this, &ATwilightArcheryCharacter::DebugLifeDown);
	PlayerInputComponent->BindAction("DebugLifeUp", IE_Pressed, this, &ATwilightArcheryCharacter::DebugLifeUp);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATwilightArcheryCharacter::OnJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ATwilightArcheryCharacter::OnStopJumping);

	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ATwilightArcheryCharacter::StartDodge);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed,  this, &ATwilightArcheryCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATwilightArcheryCharacter::StopSprinting);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATwilightArcheryCharacter::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATwilightArcheryCharacter::StopAiming);

	FInputActionBinding& toggle = PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &ATwilightArcheryCharacter::PauseGame);
	toggle.bExecuteWhenPaused = true;

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

	gameViewport->SetForceDisableSplitscreen(!gameViewport->IsSplitscreenForceDisabled());

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
	if (Controller == nullptr) return;

	FVector Direction;
	if (Value != 0.0f && !bIsDodging)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

	forwardInputDir = Direction * Value;
}

void ATwilightArcheryCharacter::MoveRight(float Value)
{
	if (Controller == nullptr) return;

	FVector Direction;
	if ( Value != 0.0f && !bIsDodging)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		UE_LOG(LogTemp, Warning, TEXT("Direction : %f | %f | %f"), Direction.X, Direction.Y, Direction.Z);
	}

	rightInputDir = Direction * Value;
}

void ATwilightArcheryCharacter::StartDodge()
{
	if (!CanDodge()) return;

	lastControlDirection = GetActorForwardVector();

	if (BowComponent->OnAim() && !BowComponent->HasShoot())
	{
		bShouldAim = true;

		OnAimingEnd();
		BowComponent->CancelAim();

		SetLastControlDirection();
	}

	bIsDodging = true;
	SetInvincible(true);

	GetCharacterMovement()->MaxWalkSpeed = dodgeSpeed;

	Stamina->StartDodging();
}

void ATwilightArcheryCharacter::StopDodge()
{
	bIsDodging = false;
	SetInvincible(false);

	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;

	if (bShouldAim)
	{
		bShouldAim = false;
		if (!GetCharacterMovement()->IsFalling())
			StartAiming();
	}
	
	Stamina->StopDodging();
}

void ATwilightArcheryCharacter::StartSprinting()
{
	if (!CanSprint()) return;

	GetCharacterMovement()->MaxWalkSpeed = sprintWalkSpeed;

	if (GetVelocity().Size() != 0.f)
		bIsSprinting = true;

	Stamina->StartSprinting();
}

void ATwilightArcheryCharacter::StopSprinting()
{
	if (BowComponent->OnAim()) return;

	bIsSprinting = false;

	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;

	Stamina->StopSprinting();
}

void ATwilightArcheryCharacter::StartAiming()
{
	if (!CanAim()) return;

	if (bIsDodging)
	{
		bShouldAim = true;
		return;
	}

	if (bIsSprinting)
		StopSprinting();

	// Set character movement control on aiming
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->MaxWalkSpeed = aimWalkSpeed;

		bUseControllerRotationRoll = true;
		bUseControllerRotationYaw = true;
	}

	// Init timer lerp camera boom
	timerArmCamera = timerArmCamera > 0.f ? delayArmBaseToAim - timerArmCamera : delayArmBaseToAim;

	Stamina->StartAiming();

	BowComponent->OnStartAiming();
}

void ATwilightArcheryCharacter::StopAiming()
{
	// Check if the bow is charged
	if (!BowComponent->OnCharge())
	{
		bShouldAim = false;
		OnAimingEnd();

		return;
	}

	// Line trace to determine the impact target point
	FHitResult hitResult;
	FVector end = FollowCamera->GetComponentLocation() + (FollowCamera->GetComponentRotation().Vector() * 100000.f);
	bool traced = GetWorld()->LineTraceSingleByChannel(hitResult, FollowCamera->GetComponentLocation(), end, ECollisionChannel::ECC_Visibility);
	FVector aimHitLocation;

	// Check if aiming on void or not
	if (traced)
		aimHitLocation = hitResult.Location;
	else
		aimHitLocation = end;

	// Shoot with bow
	FVector shootDirection = aimHitLocation - ArrowBowMesh->GetComponentLocation();
	shootDirection.Normalize();
	BowComponent->Shoot(shootDirection, ArrowBowMesh->GetComponentTransform());

	//shootFX->Activate(true);

	// Hide arrow mesh on bow socket
	ArrowBowMesh->SetHiddenInGame(true);

	if (selfController && ShootShake)
		selfController->ClientStartCameraShake(ShootShake);
}

void ATwilightArcheryCharacter::OnAimingEnd()
{
	// Check if currently aiming or dodging
	if (!BowComponent->OnAim()) return;

	BowComponent->OnEndAiming();

	// Set base character movement control
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;

		bUseControllerRotationRoll = false;
		bUseControllerRotationYaw = false;
	}

	ArrowHandMesh->SetHiddenInGame(true);

	// Init timer lerp camera boom
	timerArmCamera = timerArmCamera > 0.f ? delayArmBaseToAim - timerArmCamera : delayArmBaseToAim;

	Stamina->StopAiming();
}

void ATwilightArcheryCharacter::DrawArrow()
{
	// On Ready to shoot
	//ArrowMesh2->SetHiddenInGame(false);
	//BowComponent->StartCharging();
}

void ATwilightArcheryCharacter::TakeArrowBack()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Take Back");
	ArrowHandMesh->SetHiddenInGame(false);
}

void ATwilightArcheryCharacter::PlaceArrowOnBow()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Place Bow");
	ArrowHandMesh->SetHiddenInGame(true);
	ArrowBowMesh->SetHiddenInGame(false);
	BowComponent->StartCharging();
}

void ATwilightArcheryCharacter::OnJump()
{
	if (!CanJump()) return;

	Jump();

	Stamina->OnJump();
}

void ATwilightArcheryCharacter::OnStopJumping()
{
	if (BowComponent->OnAim()) return;

	StopJumping();

	Stamina->OnStopJumping();
}

void ATwilightArcheryCharacter::PauseGame()
{
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if (!IsValid(playerController))
		return;

	if (UGameplayStatics::IsGamePaused(GetWorld()))
	{
		playerController->SetInputMode(FInputModeGameOnly());
		playerController->SetShowMouseCursor(false);
		UGameplayStatics::SetGamePaused(GetWorld(), false);

		return;
	}

	playerController->SetInputMode(FInputModeGameAndUI());
	playerController->SetShowMouseCursor(true);
	StopAiming();
	StopSprinting();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	pauseEvent.Broadcast();
}

void ATwilightArcheryCharacter::SetInvincible(bool value)
{
	bIsInvincible = value;

	if (!bIsInvincible)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Not invincible");
}

void ATwilightArcheryCharacter::SetLastControlDirection()
{
	lastControlDirection = rightInputDir + forwardInputDir;
	lastControlDirection.Normalize();

	if (lastControlDirection == FVector::ZeroVector)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		lastControlDirection = - FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	}
}

bool ATwilightArcheryCharacter::CanSprint()
{
	return !(GetCharacterMovement()->IsFalling() || BowComponent->OnAim() || bIsDodging);
}

bool ATwilightArcheryCharacter::CanDodge()
{
	return !(GetCharacterMovement()->IsFalling());
}

bool ATwilightArcheryCharacter::CanJump()
{
	return !(BowComponent->OnAim() || bIsDodging || Stamina->currentStamina < Stamina->jumpDrain);
}

bool ATwilightArcheryCharacter::CanAim()
{
	return BowComponent->CanShoot();
}

void ATwilightArcheryCharacter::DebugLifeDown()
{
	GEngine->AddOnScreenDebugMessage(3805, 1.f, FColor::Green, FString("debugld"));
	Life->LifeDown(1);
}
void ATwilightArcheryCharacter::DebugLifeUp()
{
	GEngine->AddOnScreenDebugMessage(38305, 1.f, FColor::Green, FString("debuglu"));

	Life->LifeUp(1);
}
