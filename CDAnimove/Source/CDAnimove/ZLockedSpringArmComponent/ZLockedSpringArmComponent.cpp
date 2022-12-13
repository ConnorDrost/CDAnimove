// Fill out your copyright notice in the Description page of Project Settings.


#include "../ZLockedSpringArmComponent/ZLockedSpringArmComponent.h"

void UZLockedSpringArmComponent::SetOriginalZ(float zlocation)
{
	OriginalCharZ = zlocation;
	OriginalZ = GetRelativeLocation().Z;
}

void UZLockedSpringArmComponent::LockZAxis(float newZ, float zvelocity, float delta)
{
	float distance = newZ - OriginalCharZ;

	FVector NewLocation = GetRelativeLocation();

	if (zvelocity > 0)
	{
		NewLocation.Z -= zvelocity * delta;
		//NewLocation.Z -= distance * delta;
	}
	else if (zvelocity < 0)
	{
		NewLocation.Z -= zvelocity * delta;
		//NewLocation.Z += distance * delta;
	}

	SetRelativeLocation(NewLocation);
}

