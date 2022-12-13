// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "MainChar.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMainCharState : uint8
{
	Ninja,

	Monkey
};

USTRUCT(BlueprintType)
struct FCharacterFlipbook
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* RunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* DeathAnimation;
};

UCLASS()
class CDANIMOVE_API AMainChar : public APaperCharacter
{
	GENERATED_BODY()

public:

	//Constructor
	AMainChar();

	//Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UZLockedSpringArmComponent* SpringArmComponent;

	//Combat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class UBoxComponent* AttackBox;

	UFUNCTION()
		virtual void CombatBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void CombatEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
		bool bCanAttack;

	void ActivateCollision(UBoxComponent* Comp);
	void DeactivateCollision(UBoxComponent* Comp);

	void Attack();
	void EndAttack();
	virtual  void IfDead();
	void ResetWeaponDelay();

	float WeaponDelay;
	bool bCanBeDamaged;
	FTimerHandle timerHandle;
	FTimerHandle attackDelayHandle;
	FTimerHandle deathTimerHandle;

	//Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UHealthComponent* HealthComp;

	bool bIsAlive;

	//Movement
	void MoveRight(float value);
	virtual void Jump() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Movement")
		float Speed;

	//Sound
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	UFUNCTION()
		void MakeSomeNoise();

	//Checkpoints
	UFUNCTION()
		virtual void WorldItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void WorldItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FVector RespawnLocation;

	//Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		class USphereComponent* PickupSphere;

	class UInventoryComponent* InventoryComponent;

	void PickupObject();

	//Misc
	virtual void Tick(float DeltaSeconds) override;
	void UpdateCharacter();


protected:

	virtual void BeginPlay() override;

	//Animations

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		FCharacterFlipbook NinjaFlipbooks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		FCharacterFlipbook MonkeyFlipbooks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		FCharacterFlipbook CurrentFlipbooks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		TMap<FString, FCharacterFlipbook> AllFlipbooks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		EMainCharState CurrentMainCharState;

	void SetCorrectFlipbooks();

	virtual void UpdateAnimation();

	void ChangeMainCharState();

};
