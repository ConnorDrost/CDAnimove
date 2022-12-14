// Fill out your copyright notice in the Description page of Project Settings.


#include "../ZLockedSpringArmComponent/ZLockedSpringArmComponent.h"

void UZLockedSpringArmComponent::LockZAxis(float zvelocity, float delta)
{
	FVector NewLocation = GetRelativeLocation();

	if (zvelocity != 0)
	{ 
		NewLocation.Z -= zvelocity * delta;
	}
	else
	{

	}

	SetRelativeLocation(NewLocation);
}

