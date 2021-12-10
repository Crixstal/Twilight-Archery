// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "LifeComponent.h"
#include "TwilightArcheryCharacter.h"
#include "BossAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hitBoxHead = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxHead"));
	hitBoxHead->SetupAttachment(GetMesh(), "HeadSocket");

	hitBoxNeck = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxNeck"));
	hitBoxNeck->SetupAttachment(GetMesh(), "NeckSocket");

	hitBoxBodyBack = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBodyBack"));
	hitBoxBodyBack->SetupAttachment(GetMesh(), "Back");

	hitBoxBodyFront = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBodyFront"));
	hitBoxBodyFront->SetupAttachment(GetMesh(), "Front");


	hitBoxLeftBackLegs = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLeftBackLegs"));
	hitBoxLeftBackLegs->SetupAttachment(GetMesh(), "LeftBackLeg");

	hitBoxLeftBackFeet = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLeftBackFeet"));
	hitBoxLeftBackFeet->SetupAttachment(GetMesh(), "L_feetSocket");

	hitBoxLeftFrontLegs = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLeftFrontLegs"));
	hitBoxLeftFrontLegs->SetupAttachment(GetMesh(), "LeftFrontLeg");

	hitBoxLeftFrontArm = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLeftFrontArm"));
	hitBoxLeftFrontArm->SetupAttachment(GetMesh(), "L_UpperArmSocket");

	hitBoxSpineRight = CreateDefaultSubobject<UBoxComponent>(TEXT("SpineRight"));
	hitBoxSpineRight->SetupAttachment(GetMesh(), "R_Shoulder02Socket");

	hitBoxSpineLeft = CreateDefaultSubobject<UBoxComponent>(TEXT("SpineLeft"));
	hitBoxSpineLeft->SetupAttachment(GetMesh(), "L_Shoulder02Socket");

	hitBoxRightBackLegs = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRightBackLegs"));
	hitBoxRightBackLegs->SetupAttachment(GetMesh(), "RightBackLeg");

	hitBoxRightBackFeet = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRightBackFeet"));
	hitBoxRightBackFeet->SetupAttachment(GetMesh(), "R_feetSocket");

	hitBoxRightFrontLegs = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRightFrontLegs"));
	hitBoxRightFrontLegs->SetupAttachment(GetMesh(), "RightFrontLeg");

	hitBoxRightFrontArm = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRightFrontArm"));
	hitBoxRightFrontArm->SetupAttachment(GetMesh(), "R_UpperArmSocket");


	hitBoxBasicAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBasicAttack"));
	hitBoxBasicAttack->SetupAttachment(GetMesh(), "R_HandSocket"); 
	
	hitBoxHornAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxHornAttack"));
	hitBoxHornAttack->SetupAttachment(GetMesh(), "HeadSocket");
	
	hitZoneAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxZoneAttack"));
	hitZoneAttack->SetupAttachment(RootComponent);
	
	Life = CreateDefaultSubobject<ULifeComponent>(TEXT("LifeComponent"));

	Life->maxLife = 700;
	Life->currentLife = Life->maxLife;
	damage = 15;
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	hitBoxHead->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxBodyBack->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxBodyFront->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxSpineLeft->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxSpineRight->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxLeftBackLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxLeftBackFeet->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxLeftFrontLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxLeftFrontArm->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxRightBackLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxRightBackFeet->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxRightFrontLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxRightFrontArm->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);

	hitBoxBasicAttack->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxBasicAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	
	hitBoxHornAttack->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxHornAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	hitZoneAttack->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitZoneAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	world = GetWorld();

	for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
	{
		AController* pl = Cast<AController>(*iter);
		Players.Add(pl);
	}

	GetCharacterMovement()->MaxWalkSpeed = 430.f;

	Life->InitActor(this);
	Life->deathEvent.AddDynamic(this, &ABossCharacter::OnBossDeath);
	Life->healthUpdate.AddDynamic(this, &ABossCharacter::OnBossTakeHit);

}

void ABossCharacter::OnBossTakeHit()
{

	if (
		(Life->currentLife <= (Life->maxLife * 0.3)
		|| (Life->currentLife <= (Life->maxLife * 0.7) && Life->currentLife >= (Life->maxLife * 0.65))
		|| (Life->currentLife <= (Life->maxLife * 0.5) && Life->currentLife >= (Life->maxLife * 0.45)))
		&& !isInRage && cdRaging == 40)
	{
		isInRage = true;
		GetWorldTimerManager().SetTimer(TimerHandleRage, this, &ABossCharacter::Raging, 1.f, true);
	}
}

void ABossCharacter::Raging()
{
	if (cdRaging == 40)
	{
		damage = damage * 2;
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
		cdRaging--;
	}
	else if (cdRaging == 0)
	{
		isInRage = false;
		damage = damage/2;
		cdRaging = 40;
		GetCharacterMovement()->MaxWalkSpeed = 430.f;
		GetWorldTimerManager().ClearTimer(TimerHandleKFOT);
	}
	else
		cdRaging--;
}

void ABossCharacter::OnBossDeath()
{

}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComp->GetName() == "BoxBasicAttack" || OverlappedComp->GetName() == "BoxHornAttack" || OverlappedComp->GetName() == "BoxZoneAttack")
	{
		if (OtherActor->Tags.Num() > 0)
		{
			if (OtherActor->ActorHasTag(TEXT("Player")))
			{
				ATwilightArcheryCharacter* player = Cast<ATwilightArcheryCharacter>(OtherActor);
				player->Life->LifeDown(damage);
				FVector normal = GetActorLocation() - player->GetActorLocation();
				player->OnHit(normal);
				if (player == target && player->Life->currentLife <= 0)
					targetIsDead = true;
			}
		}
	}
}

void ABossCharacter::KeepFocusOnTarget()
{
	if (focustime > 0)
		focustime -= 1.f;
	else
	{
		haveATarget = false;
		GetWorldTimerManager().ClearTimer(TimerHandleKFOT);
	}
}

void ABossCharacter::Attacking()
{
	if (zoneAttack == true)
	{
		if (timeZonAtt >= 1.48f && timeZonAtt <= 1.52f)
			hitZoneAttack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		else if (timeZonAtt >= 2.f)
		{
			hitZoneAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			ABossCharacter::StopZoneAttack();
		}

		timeZonAtt += 0.1f;
	}
	else if (basicAttack == true)
	{
		if (isInRage)
		{
			if (timeBasAtt >= 0.68f && timeBasAtt <= 0.72f)
				hitBoxBasicAttack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			else if (timeBasAtt >= 0.98f && timeBasAtt <= 1.02f)
				hitBoxBasicAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			else if (timeBasAtt >= 1.82F)
				ABossCharacter::StopBasicAttack();
		}
		else
		{
			if (timeBasAtt >= 1.28f && timeBasAtt <= 1.32f)
				hitBoxBasicAttack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			else if (timeBasAtt >= 1.78f && timeBasAtt <= 1.82f)
				hitBoxBasicAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			else if (timeBasAtt >= 3.33f)
				ABossCharacter::StopBasicAttack();
		}

		timeBasAtt += 0.1f;
	}
	else if (hornAttack == true)
	{
		if (isInRage)
		{
			if (timeHorAtt >= 0.28f && timeHorAtt <= 0.32f)
				hitBoxHornAttack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			else if (timeHorAtt >= 0.78f && timeHorAtt <= 0.82f)
				hitBoxHornAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			else if (timeHorAtt >= 1.02F)
				ABossCharacter::StopHornAttack();
		}
		else
		{
			if (timeHorAtt >= 0.58f && timeHorAtt <= 0.62f)
				hitBoxHornAttack->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			else if (timeHorAtt >= 1.48f && timeHorAtt <= 1.52f)
				hitBoxHornAttack->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			else if (timeHorAtt >= 2.f)
				ABossCharacter::StopHornAttack();
		}

		timeHorAtt += 0.1f;
	}
	else if (rockAttack == true)
	{
		if (timeRockAtt >= 4.f)
			ABossCharacter::StopRockAttack();

		timeRockAtt += 0.1f;
	}
}

void ABossCharacter::ZoneAttack()
{
	isAttacking = true;
	zoneAttack = true;
	GetWorldTimerManager().SetTimer(AttZone, this, &ABossCharacter::Attacking, 0.1f, true);
	FVector FacingVector = { target->GetActorLocation().X - GetActorLocation().X, target->GetActorLocation().Y - GetActorLocation().Y, 0 };
	FRotator FacingRotator = FacingVector.Rotation();
	SetActorRotation(FacingRotator, ETeleportType::None);

}

void ABossCharacter::StopZoneAttack()
{
	GetWorldTimerManager().ClearTimer(AttZone);
	isAttacking = false;
	zoneAttack = false;
	isChasing = false;
	timeZonAtt = 0.f;
}

void ABossCharacter::HornAttack()
{
	isAttacking = true;
	hornAttack = true;
	GetWorldTimerManager().SetTimer(AttHorn, this, &ABossCharacter::Attacking, 0.1f, true);
	FVector FacingVector = { target->GetActorLocation().X - GetActorLocation().X, target->GetActorLocation().Y - GetActorLocation().Y, 0 };
	FRotator FacingRotator = FacingVector.Rotation();
	SetActorRotation(FacingRotator, ETeleportType::None);

}

void ABossCharacter::StopHornAttack()
{
	GetWorldTimerManager().ClearTimer(AttHorn);
	isAttacking = false;
	hornAttack = false;
	isChasing = false;
	chooseRdAtt = true;
	timeHorAtt = 0.f;
}

void ABossCharacter::BasicAttack()
{
	isAttacking = true;
	basicAttack = true;
	GetWorldTimerManager().SetTimer(AttBasic, this, &ABossCharacter::Attacking, 0.1f, true);
	FVector FacingVector = {target->GetActorLocation().X - GetActorLocation().X, target->GetActorLocation().Y - GetActorLocation().Y, 0};
	FRotator FacingRotator = FacingVector.Rotation();
	SetActorRotation(FacingRotator, ETeleportType::None);
}

void ABossCharacter::StopBasicAttack()
{
	GetWorldTimerManager().ClearTimer(AttBasic);
	isAttacking = false;
	basicAttack = false;
	isChasing = false;
	chooseRdAtt = true;
	timeBasAtt = 0.f;
}

void ABossCharacter::RockAttack()
{
	isAttacking = true;
	rockAttack = true;
	GetWorldTimerManager().SetTimer(AttRock, this, &ABossCharacter::Attacking, 0.1f, true);
	FVector FacingVector = { target->GetActorLocation().X - GetActorLocation().X, target->GetActorLocation().Y - GetActorLocation().Y, 0 };
	FRotator FacingRotator = FacingVector.Rotation();
	SetActorRotation(FacingRotator, ETeleportType::None);
	rockAttackEvent.Broadcast();
}

void ABossCharacter::StopRockAttack()
{
	GetWorldTimerManager().ClearTimer(AttRock);
	isAttacking = false;
	rockAttack = false;
	isChasing = false;
	chooseRdAtt = true;
	timeRockAtt = 0.f;
}


void ABossCharacter::OnDeath()
{
	Cast<ABossAIController>(GetController())->OnDeath();

	bIsDead = true;

	GetCharacterMovement()->StopMovementImmediately();
}