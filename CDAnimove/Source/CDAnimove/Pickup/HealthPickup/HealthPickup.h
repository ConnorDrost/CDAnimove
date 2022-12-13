// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Pickup.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class CDANIMOVE_API AHealthPickup : public APickup
{
	GENERATED_BODY()

public:
	
	AHealthPickup();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float BonusHealth;
	
};
