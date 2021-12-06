// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "BossCharacter.h"
#include "LifeComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule collider");
	RootComponent = CapsuleComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile Movement");
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComponent->OnComponentHit.AddDynamic(this, &AArrow::OnCompHit);
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArrow::Initialize(FVector velocity)
{
	ProjectileComponent->Velocity = velocity;
	ProjectileComponent->bRotationFollowsVelocity = true;
	ProjectileComponent->ProjectileGravityScale = 0.6;
}

void AArrow::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GEngine->AddOnScreenDebugMessage(-864, 5.f, FColor::Emerald, "ArrowHit");
	if (OtherActor->ActorHasTag(TEXT("Enemy")))
	{
		GEngine->AddOnScreenDebugMessage(78663, 5.f, FColor::Emerald, OtherActor->GetName());
		//TODO: Send to enemy what body part was hit to calculate damage res
		ABossCharacter* enemy = Cast<ABossCharacter>(OtherActor);
		enemy->Life->LifeDown(10);
		//AttachToActor()
		AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);
		//Mesh->SetupAttachment(OtherComp);
		//this->SetRootComponent(OtherComp);
	}
	//CapsuleComponent->DestroyComponent();
	//ProjectileComponent->DestroyComponent();
}
