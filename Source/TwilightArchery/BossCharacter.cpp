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


	
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	hitBoxHead->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxBody->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);
	hitBoxLegs->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnOverlapBegin);


}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}