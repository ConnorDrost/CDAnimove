// Fill out your copyright notice in the Description page of Project Settings.


#include "../Checkpoint/Checkpoint.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckpointSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Actor Sprite");
	CheckpointSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("SceneRoot");
	CollisionBox->SetCollisionProfileName("OverlapAll");
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionBox->SetupAttachment(CheckpointSprite);

	IsChecked = false;
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

