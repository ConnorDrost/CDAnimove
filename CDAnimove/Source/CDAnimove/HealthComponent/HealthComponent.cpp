// Fill out your copyright notice in the Description page of Project Settings.


#include "../HealthComponent/HealthComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


void UHealthComponent::IncreaseHealth(float value)
{
	if (CurrentHealth += value > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	else
	{
		CurrentHealth += value;
	}
}

void UHealthComponent::DecreaseHealth(float value)
{
	float NewHealth = CurrentHealth - value;

	if (NewHealth > 0 )
	{
		CurrentHealth -= value;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "HIT");
	}
	else
	{
		CurrentHealth = 0;
	}
}

bool UHealthComponent::isAlive()
{
	if (CurrentHealth <= 0)
	{
		return false;
	}
	else 
	{
		return true;
	}

}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

