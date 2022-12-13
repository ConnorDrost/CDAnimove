// Fill out your copyright notice in the Description page of Project Settings.


#include "../InventoryComponent/InventoryComponent.h"
#include "../Pickup/Pickup.h"
#include "../Pickup/MonkeyPickup/MonkeyPickup.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UInventoryComponent::AddToInventory(APickup* pickup)
{
	Cast<APickup>(pickup);

	Inventory.Add(pickup);

	pickup->Disable();
}

bool UInventoryComponent::CheckInventoryForMonkey()
{
	if (Inventory.Num() > 0)
	{
		for (int i = 0; i < Inventory.Num(); i++)\
		{
			if (Inventory[i]->IsA<AMonkeyPickup>())
			{
				return true;
			}
		}
	}

	return false;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

