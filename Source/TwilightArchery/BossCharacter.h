// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "TwilightArcheryCharacter.h"
#include "BossCharacter.generated.h"

UCLASS()
class TWILIGHTARCHERY_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Components HeadBox")
		class UBoxComponent* hitBoxHead;

	UPROPERTY(VisibleAnywhere, Category = "Components BodyBox")
		class UBoxComponent* hitBoxBody;

	UPROPERTY(VisibleAnywhere, Category = "Components LegBox")
		class UBoxComponent* hitBoxLegs;

	/*UPROPERTY(VisibleAnywhere, Category = "lifeEnemy")
		int life = 200;*/

	AActor* target;
	bool haveATarget = false;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle TimerHandleKFOT;
	int focusingTimeMin = 3.f;
	int focusingTimeMax = 8.f;
	int focustime = 0.f;
	void KeepFocusOnTarget();
	
	UWorld* world;
	TArray<AController*> Players;

	FTimerHandle TimerHandleAtt;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boolean Attack")
		bool isAttacking = false;
	int attackingTime = 4.f;
	void Attacking();

	
	UPROPERTY(VisibleAnywhere, Category = "Components BasicAttack")
		class UBoxComponent* hitBoxBasicAttack;

	void BasicAttack();
	void StopBasicAttack();
};
