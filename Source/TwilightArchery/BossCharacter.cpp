// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hitBoxHead = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxHead"));
	hitBoxHead->SetupAttachment(GetMesh(), "HeadSocket");

	hitBoxBodyBack = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBodyBack"));
	hitBoxBodyBack->SetupAttachment(GetMesh(), "Back");

	hitBoxBodyFront = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBodyFront"));
	hitBoxBodyFront->SetupAttachment(GetMesh(), "Front");

	hitBoxLeftBackLegs = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLeftBackLegs"));
	hitBoxLeftBackLegs->SetupAttachment(GetMesh(), "LeftBackLeg");

	hitBoxLeftFrontLegs = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLeftFrontLegs"));
	hitBoxLeftFrontLegs->SetupAttachment(GetMesh(), "LeftFrontLeg");

	hitBoxRightBackLegs = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRightBackLegs"));
	hitBoxRightBackLegs->SetupAttachment(GetMesh(), "RightBackLeg");

	hitBoxRightFrontLegs = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRightFrontLegs"));
	hitBoxRightFrontLegs->SetupAttachment(GetMesh(), "RightFrontLeg");

	hitBoxBasicAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBasicAttack"));
	hitBoxBasicAttack->SetupAttachment(GetMesh(), "R_HandSocket");
	
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	hitBoxHead->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxBodyBack->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxBodyFront->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxLeftBackLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxLeftFrontLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxRightBackLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxRightFrontLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);

	world = GetWorld();

	for (FConstPlayerControllerIterator iter = world->GetPlayerControllerIterator(); iter; ++iter)
	{
		AController* pl = Cast<AController>(*iter);
		Players.Add(pl);
	}
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlappedComp->GetName() == "BoxBasicAttack")
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("hit %d"), *OtherActor->GetName()));
		if(OtherComp->GetName() == "CollisionCylinder" && OtherActor->Tags[0] == "Player")
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("hit player")));
		}
	}
}

void ABossCharacter::KeepFocusOnTarget()
{
	if (focustime > 0)
	{
		focustime -= 1.f;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("-1 seconde")));

	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("EndCoolDown")));
		haveATarget = false;
		GetWorldTimerManager().ClearTimer(TimerHandleKFOT);
	}
}

void ABossCharacter::Attacking()
{
	if (attackingTime > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("-1 seconde")));
		attackingTime -= 1.f;
	}
	else
	{
		GEngine->ClearDebugDisplayProperties();
		ABossCharacter::StopBasicAttack();
	}
}

void ABossCharacter::BasicAttack()
{
	hitBoxBasicAttack->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	isAttacking = true;
	GetWorldTimerManager().SetTimer(TimerHandleAtt, this, &ABossCharacter::Attacking, 1.f, true);
	FVector FacingVector = {target->GetActorLocation().X - GetActorLocation().X, target->GetActorLocation().Y - GetActorLocation().Y, 0};
	FRotator FacingRotator = FacingVector.Rotation();
	SetActorRotation(FacingRotator, ETeleportType::None);

	/*GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("HAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA")));*/

}

void ABossCharacter::StopBasicAttack()
{
	GetWorldTimerManager().ClearTimer(TimerHandleAtt);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("STOP ATTACK")));
	hitBoxBasicAttack->OnComponentBeginOverlap.RemoveDynamic(this, &ABossCharacter::OnOverlapBegin);
	isAttacking = false;
	attackingTime = 4.f;
}