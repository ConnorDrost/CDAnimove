// Fill out your copyright notice in the Description page of Project Settings.


#include "../Moving Platforms/MovingPlatform.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("SceneRoot");
	CollisionBox->SetCollisionProfileName("BlockAllDynamic");
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetRootComponent(CollisionBox);

	PlatformSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Actor Visual");
	PlatformSprite->SetCollisionProfileName("NoCollision");
	PlatformSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlatformSprite->SetGenerateOverlapEvents(false);
	PlatformSprite->SetupAttachment(RootComponent);

}

void AMovingPlatform::Move(float delta)
{
	if (Horizontal == true || Vertical == true)
	{
		DistanceTraveled += abs(Speed * delta);

		if (DistanceTraveled >= TravelDistance)
		{
			ChangeDirection();
		}

		if (Horizontal == true)
		{
			if (!Backwards)
			{
				FVector NewLocation = FVector(GetActorLocation().X + Speed * delta, GetActorLocation().Y, GetActorLocation().Z);

				SetActorLocation(NewLocation);
			}
			else
			{
				FVector NewLocation = FVector(GetActorLocation().X - Speed * delta, GetActorLocation().Y, GetActorLocation().Z);

				SetActorLocation(NewLocation);
			}
		}

		if (Vertical == true)
		{
			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + Speed * delta);

			SetActorLocation(NewLocation);
		}
	}
	else
	{

	}
}

void AMovingPlatform::ChangeDirection()
{
	Speed *= -1;
	DistanceTraveled = 0;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

