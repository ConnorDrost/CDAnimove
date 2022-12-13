// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class CDANIMOVE_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	UPROPERTY(EditAnywhere, Category = "Actor Visual")
		class UPaperSpriteComponent* PlatformSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
		class UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		float TravelDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		bool Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		bool Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform")
		bool Backwards;


private:

	void Move(float delta);

	void ChangeDirection();

	float DistanceTraveled = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
