// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Arrow.generated.h"

UCLASS()
class TWILIGHTARCHERY_API AArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArrow();

protected:
	UPROPERTY(Category = Actor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;
	UPROPERTY(Category = Actor, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComponent;

	UProjectileMovementComponent* ProjectileComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void Initialize(FVector velocity);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
