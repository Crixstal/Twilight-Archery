// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "TwilightArcheryCharacter.h"
#include "BossCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRockAttackEvent);

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


	UFUNCTION()
		void OnBossTakeHit();

	UFUNCTION(BlueprintCallable)
		void OnDeath();

	UFUNCTION()
		void OnBossDeath();

	UPROPERTY(BlueprintAssignable, Category = "SelfParameters|Delegates")
		FRockAttackEvent rockAttackEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters|Components")
		class ULifeComponent* Life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components HeadBox")
		class UBoxComponent* hitBoxHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components NeckBox")
		class UBoxComponent* hitBoxNeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components BodyBoxBack")
		class UBoxComponent* hitBoxBodyBack; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components BodyBoxFront")
		class UBoxComponent* hitBoxBodyFront;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components SpineRight")
		class UBoxComponent* hitBoxSpineRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components SpineLeft")
		class UBoxComponent* hitBoxSpineLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components LeftBackLegBox")
		class UBoxComponent* hitBoxLeftBackLegs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components LeftBackFeetBox")
		class UBoxComponent* hitBoxLeftBackFeet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components RightBackLegBox")
		class UBoxComponent* hitBoxRightBackLegs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components RightBackFeetBox")
		class UBoxComponent* hitBoxRightBackFeet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components LeftFrontLegBox")
		class UBoxComponent* hitBoxLeftFrontLegs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components LeftFrontArmBox")
		class UBoxComponent* hitBoxLeftFrontArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components RightFrontLegBox")
		class UBoxComponent* hitBoxRightFrontLegs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components RightFrontArmBox")
		class UBoxComponent* hitBoxRightFrontArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
		TMap<FName, float> multipliers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Target")
	AActor* target;
	bool haveATarget = false;

	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle TimerHandleKFOT;
	int focusingTimeMin = 3.f;
	int focusingTimeMax = 8.f;
	int focustime = 0.f;
	void KeepFocusOnTarget();

	int dpsFromTarget = 0;
	int dpsFromTheOtherPlayer = 0;
	bool targetIsDead = false;

	UWorld* world;
	TArray<AController*> Players;

	void Attacking();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
	bool isAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
	bool isChasing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
	bool isRotating = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters")
	bool isInRage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SelfParameters\|Booleans")
	bool bIsDead = false;

	FTimerHandle TimerHandleRage;
	void Raging();
	int cdRaging = 40;
	int damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components Choose Rd Attack")
		bool chooseRdAtt = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components Selected Attack")
		int selectedAttack = 0;

	//ZoneAttack
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components ZoneAttack")
			class UBoxComponent* hitZoneAttack;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boolean Zone Attack")
			bool zoneAttack = false;

		void ZoneAttack();
		void StopZoneAttack();
		float timeZonAtt = 0;
		FTimerHandle AttZone;

	//BasicAttack
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components BasicAttack")
			class UBoxComponent* hitBoxBasicAttack;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boolean Basic Attack")
			bool basicAttack = false;

		void BasicAttack();
		void StopBasicAttack();
		float timeBasAtt = 0;
		FTimerHandle AttBasic;

	//HornAttack
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components HornAttack")
			class UBoxComponent* hitBoxHornAttack;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boolean Horn Attack")
			bool hornAttack = false;

		void HornAttack();
		void StopHornAttack();
		float timeHorAtt = 0;
		FTimerHandle AttHorn;

	//RockAttack
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boolean Rock Attack")
			bool rockAttack = false;

		void RockAttack();
		void StopRockAttack();
		float timeRockAtt = 0;
		FTimerHandle AttRock;
};
