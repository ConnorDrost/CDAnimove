// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UEGameState.generated.h"

/**
 * 
 */
UCLASS()
class CDANIMOVE_API AUEGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AUEGameState();

	float PlayerScore = 0;

	UFUNCTION(BlueprintCallable, Category = "Score")
		int GetPlayerScore() { return PlayerScore; }
	
};
