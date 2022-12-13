// Fill out your copyright notice in the Description page of Project Settings.


#include "../Pickup/Pickup.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionSphere->SetSimulatePhysics(false);
	CollisionSphere->SetCollisionProfileName("OverlapAllDynamic");
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(CollisionSphere);

	PickupSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Actor Visual");
	PickupSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupSprite->SetupAttachment(RootComponent);
}

void APickup::Disable()
{
	SetActorHiddenInGame(true);
	CollisionSphere->SetActive(false);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

