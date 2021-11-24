// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	hitBoxHead = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxHead"));
	hitBoxHead->SetupAttachment(RootComponent);

	hitBoxBody = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBody"));
	hitBoxBody->SetupAttachment(RootComponent);

	hitBoxLegs = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLeftLeg"));
	hitBoxLegs->SetupAttachment(RootComponent);

	hitBoxBasicAttack = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxBasicAttack"));
	hitBoxBasicAttack->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	hitBoxHead->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxBody->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);

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
}

void ABossCharacter::StopBasicAttack()
{
	GetWorldTimerManager().ClearTimer(TimerHandleAtt);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("STOP ATTACK")));
	hitBoxBasicAttack->OnComponentBeginOverlap.RemoveDynamic(this, &ABossCharacter::OnOverlapBegin);
	isAttacking = false;
	attackingTime = 4.f;
}