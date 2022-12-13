// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class CDANIMOVE_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		class UPaperSpriteComponent* PickupSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
		class USphereComponent* CollisionSphere;

	UFUNCTION()
		void Disable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
