// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../MainChar/MainChar.h"
#include "../CDAnimove.h"

#include "AICharacterGuard.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,

	Alerted
};

UCLASS()
class CDANIMOVE_API AAICharacterGuard : public AMainChar
{
	GENERATED_BODY()

public:

	AAICharacterGuard();

	virtual void Tick(float DeltaTime) override;

	void SetGuardState(EAIState NewState);

	void SetTargetActor(AActor* target) { TargetActor = target; }

protected:

	virtual void BeginPlay() override;

	//Patrolling
	UPROPERTY(VisibleAnywhere, Category = "AI")
		EAIState GuardState;

	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor* FirstPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol"))
		AActor* SecondPatrolPoint;

	AActor* CurrentPatrolPoint = nullptr;

	UFUNCTION()
		void MoveToNextPatrolPoint();

	//PawnSensing
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
		void OnPawnSeen(APawn* SeenPawn);

	//Combat
	UFUNCTION()
		void CheckAggression();

	virtual void IfDead() override;

	virtual void CombatBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void CombatEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	//Animation
	virtual void UpdateAnimation() override;

private:

	AController* pController;
	AActor* TargetActor;
};
