// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Arrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHitDelegate, bool, hasHitEnemy, int, damage);

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
	class ATwilightArcheryCharacter* player;

	UPROPERTY(BlueprintAssignable, Category = "SelfParameters|Delegates")
		FHitDelegate hitDelegate;

	void Initialize(ATwilightArcheryCharacter* inPlayer, FVector velocity, float charge = 1.f);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
		int damage = 10;

	float GetMultiplier(const FName& key, const TMap<FName, float>& multipliers);

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
