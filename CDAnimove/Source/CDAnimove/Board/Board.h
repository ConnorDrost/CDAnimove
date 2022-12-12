// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Board.generated.h"

UCLASS()
class CDANIMOVE_API ABoard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoard();

	UPROPERTY(EditAnywhere, Category = "Actor Visual")
		class UPaperSpriteComponent* BoardSprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
		class UBoxComponent* SideBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
		class UBoxComponent* TopBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom")
		class UBoxComponent* BottomBorder;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
