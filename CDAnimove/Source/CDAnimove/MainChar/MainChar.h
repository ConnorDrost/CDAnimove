// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "MainChar.generated.h"

/**
 * 
 */
UCLASS()
class CDANIMOVE_API AMainChar : public APaperCharacter
{
	GENERATED_BODY()

public:

	//Constructor
	AMainChar();

	//Animations
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

	virtual void UpdateAnimation();

	//Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* CameraComponent;

	FName CameraComponentName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		class USpringArmComponent* SpringArmComponent;

	FName SpringArmComponentName;

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

	virtual void Attack();
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		class UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	UFUNCTION()
		void MakeSomeNoise();

	//Checkpoints
	UFUNCTION()
		virtual void CPBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void CPEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FVector RespawnLocation;

	//Misc
	virtual void Tick(float DeltaSeconds) override;
	void UpdateCharacter();


protected:

	virtual void BeginPlay() override;

};
