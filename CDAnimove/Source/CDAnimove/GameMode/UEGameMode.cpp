// Fill out your copyright notice in the Description page of Project Settings.


#include "../GameMode/UEGameMode.h"
#include "Kismet/GameplayStatics.h"

void AUEGameMode::BeginPlay()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (controller)
	{
		FString command = "show collision";
		//controller->ConsoleCommand(command, true);
		controller->bShowMouseCursor = true;
		controller->bEnableClickEvents = true;
		controller->bEnableMouseOverEvents = true;
	}
}
