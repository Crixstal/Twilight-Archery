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

ATwilightArcheryCharacter::ATwilightArcheryCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	// set our turn rates for input
	BaseTurnRate = 30.f;
	BaseLookUpRate = 30.f;

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
	DodgeCapsule->SetupAttachment(GetMesh(), FName("BaseLocation"));

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
	DodgeCapsule->SetWorldRotation(FRotator());
	DodgeCapsule->SetRelativeLocation(FVector());

	Stamina->InitPlayer(this);
	Life->InitActor(this);
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
		AddMovementInput(lastControlDirection, 1.f);
	}

	if (bIsHit)
		AddMovementInput(lastControlDirection, 1.f);
}

void ATwilightArcheryCharacter::UpdateCameraBoom()
{
	if (timerArmCamera > 0.f)
	{
		float t = timerArmCamera / delayArmBaseToAim;
		t = BowComponent->bIsAiming ? 1.f - t : t;

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

}

void ATwilightArcheryCharacter::TurnAtRate(float Rate)
{
	float newRate = -1.f + (1.f + 1.f) * ((Rate + 0.5f) / (0.5f + 0.5f));
	// calculate delta for this frame from the rate information
	AddControllerYawInput(newRate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATwilightArcheryCharacter::LookUpAtRate(float Rate)
{
	float newRate = -1.f + (1.f + 1.f) * ((Rate + 0.5f) / (0.5f + 0.5f));
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(newRate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATwilightArcheryCharacter::MoveForward(float Value)
{
	if (Controller == nullptr) return;

	FVector Direction;
	if (Value != 0.0f && !bIsDodging && !bIsHit && !bIsClimbing)
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
	if ( Value != 0.0f && !bIsDodging && !bIsHit && !bIsClimbing)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		//UE_LOG(LogTemp, Warning, TEXT("Direction : %f | %f | %f"), Direction.X, Direction.Y, Direction.Z);
	}

	rightInputDir = Direction * Value;
}

void ATwilightArcheryCharacter::StartDodge()
{
	if (!CanDodge()) return;

	lastControlDirection = GetActorForwardVector();

	if (BowComponent->bIsAiming && !BowComponent->bHasShoot)
	{

		OnAimingEnd();
		BowComponent->CancelAim();

		BowComponent->bShouldAim = true;

		SetLastControlDirection();
	}

	bIsDodging = true;
	SetInvincible(true);

	dodgeEvent.Broadcast();

	GetCharacterMovement()->MaxWalkSpeed = dodgeSpeed;

	Stamina->StartDodging();
}

void ATwilightArcheryCharacter::StopDodge()
{
	bIsDodging = false;
	SetInvincible(false);

	Stamina->StopDodging();

	if (BowComponent->bShouldAim)
	{
		BowComponent->bShouldAim = false;
		if (!GetCharacterMovement()->IsFalling())
			StartAiming();

		return;
	}

	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
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
	if (!bIsSprinting || BowComponent->bIsAiming) return;

	bIsSprinting = false;

	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;

	Stamina->StopSprinting();
}

void ATwilightArcheryCharacter::StartAiming()
{
	if (!CanAim()) return;

	if (BowComponent->bIsAiming || bIsDodging)
	{
		BowComponent->bShouldAim = true;
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
	if (!BowComponent->bIsCharging)
	{
		BowComponent->bShouldAim = false;
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
	BowComponent->Shoot(this, shootDirection, ArrowBowMesh->GetComponentTransform());

	//shootFX->Activate(true);

	// Hide arrow mesh on bow socket
	ArrowBowMesh->SetHiddenInGame(true);

	if (selfController && ShootShake)
		selfController->ClientStartCameraShake(ShootShake);
}

void ATwilightArcheryCharacter::OnAimingEnd()
{
	// Check if currently aiming or dodging
	if (!BowComponent->CanEndAiming()) return;

	if (BowComponent->bShouldAim)
	{
		BowComponent->OnEndAiming();
		BowComponent->OnStartAiming();
		return;
	}

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

}

void ATwilightArcheryCharacter::TakeArrowBack()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Take Back");
	ArrowHandMesh->SetHiddenInGame(false);
}

void ATwilightArcheryCharacter::PlaceArrowOnBow()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Place Bow");
	ArrowHandMesh->SetHiddenInGame(true);
	ArrowBowMesh->SetHiddenInGame(false);
	BowComponent->StartCharging();
}

void ATwilightArcheryCharacter::OnJump()
{
	if (!CanJump()) return;

	jumpEvent.Broadcast();

	Jump();
	bIsJumping = true;

	Stamina->OnJump();
}

void ATwilightArcheryCharacter::OnStopJumping()
{
	if (!bIsJumping || BowComponent->bIsAiming) return;

	StopJumping();
	bIsJumping = false;

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
	Life->SetInvincibility(value, 0.f, true);
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
	return !(GetCharacterMovement()->IsFalling() || BowComponent->bIsAiming || bIsDodging || bIsHit || bIsClimbing || bIsDead);
}

bool ATwilightArcheryCharacter::CanDodge()
{
	return !(GetCharacterMovement()->IsFalling() || bIsHit || Stamina->currentStamina < Stamina->dodgeDrain || bIsClimbing || bIsDead);
}

bool ATwilightArcheryCharacter::CanJump()
{
	return !(BowComponent->bIsAiming || bIsDodging || Stamina->currentStamina < Stamina->jumpDrain || bIsHit || bIsClimbing || bIsDead);
}

bool ATwilightArcheryCharacter::CanAim()
{
	return BowComponent->CanShoot() && !bIsHit && Stamina->currentStamina >= Stamina->aimDrain && !bIsClimbing && !bIsDead;
}

void ATwilightArcheryCharacter::DebugLifeDown()
{
	Life->LifeDown(1);
}
void ATwilightArcheryCharacter::DebugLifeUp()
{

	Life->LifeUp(1);
}

void ATwilightArcheryCharacter::OnHit(const FVector& Normal)
{
	if (Life->bIsInvincible || bIsDead) return;

	if (BowComponent->bIsAiming)
	{
		OnAimingEnd();
		BowComponent->CancelAim();
	}

	if (bIsDodging)
		StopDodge();

	if (bIsSprinting)
		StopSprinting();

	bIsHit = true;

	Life->SetInvincibility(true, 0.f, true);

	GetCharacterMovement()->MaxWalkSpeed = hitWalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	FVector actorForward = GetActorForwardVector();
	FVector normalHit = Normal;
	normalHit.Z = 0.f;
	normalHit.Normalize();

	if (normalHit.Equals(FVector::ZeroVector, 0.001f))
	{
		hitDirection = FVector(0.f, 1.f, 0.f);
		onHitEvent.Broadcast();

		return;
	}

	float absAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(actorForward, normalHit)));

	if (absAngle < 45.f)
	{
		hitDirection = FVector(0.f, 1.f, 0.f);
		lastControlDirection = -actorForward;
	}
	else if (absAngle < 135.f)
	{
		FVector zAxis = FVector::CrossProduct(normalHit, actorForward);
		hitDirection = FVector(1.f, 0.f, 0.f) * FMath::Sign(zAxis.Z);
		lastControlDirection = GetActorRightVector() * hitDirection.X;
	}
	else
	{
		hitDirection = FVector(0.f, -1.f, 0.f);
		lastControlDirection = actorForward;
	}

	onHitEvent.Broadcast();
}

void ATwilightArcheryCharacter::OnEndHit()
{
	bIsHit = false;

	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Life->SetInvincibility(false, 0.f);
}

void ATwilightArcheryCharacter::OnStaminaRegen()
{
	GetCharacterMovement()->MaxWalkSpeed = baseWalkSpeed;
}

void ATwilightArcheryCharacter::OnStaminaEmpty()
{
	if (BowComponent->bIsAiming)
	{
		OnAimingEnd();
		BowComponent->CancelAim();
	}

	if (bIsSprinting)
		StopSprinting();

	GetCharacterMovement()->MaxWalkSpeed = lowStaminaWalkSpeed;
}

void ATwilightArcheryCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
