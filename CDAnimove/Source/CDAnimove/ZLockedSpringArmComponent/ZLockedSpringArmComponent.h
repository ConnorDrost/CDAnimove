// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "ZLockedSpringArmComponent.generated.h"

/**
 * 
 */
UCLASS()
class CDANIMOVE_API UZLockedSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	void  LockZAxis(float zvelocity, float delta);

private:

};
