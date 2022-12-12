// Fill out your copyright notice in the Description page of Project Settings.


#include "../Board/Board.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ABoard::ABoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoardSprite = CreateDefaultSubobject<UPaperSpriteComponent>("Actor Visual");
	BoardSprite->SetCollisionProfileName("NoCollision");
	BoardSprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoardSprite->SetGenerateOverlapEvents(false);

	SetRootComponent(BoardSprite);

	SideBorder = CreateDefaultSubobject<UBoxComponent>("SideBorder");
	SideBorder->SetBoxExtent(FVector(10, 10, 10));
	SideBorder->SetCollisionProfileName("BlockAllDynamic");
	SideBorder->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SideBorder->GetBodyInstance()->bLockRotation = true;
	SideBorder->GetBodyInstance()->bLockTranslation = true;
	SideBorder->SetSimulatePhysics(false);
	SideBorder->SetupAttachment(RootComponent);

	BottomBorder = CreateDefaultSubobject<UBoxComponent>("BottomBorder");
	BottomBorder->SetBoxExtent(FVector(10, 10, 10));
	BottomBorder->SetCollisionProfileName("BlockAllDynamic");
	BottomBorder->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BottomBorder->GetBodyInstance()->bLockRotation = true;
	BottomBorder->GetBodyInstance()->bLockTranslation = true;
	BottomBorder->SetSimulatePhysics(false);
	BottomBorder->SetupAttachment(RootComponent);

	TopBorder = CreateDefaultSubobject<UBoxComponent>("TopBorder");
	TopBorder->SetBoxExtent(FVector(10, 10, 10));
	TopBorder->SetCollisionProfileName("BlockAllDynamic");
	TopBorder->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TopBorder->GetBodyInstance()->bLockRotation = true;
	TopBorder->GetBodyInstance()->bLockTranslation = true;
	TopBorder->SetSimulatePhysics(false);
	TopBorder->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

