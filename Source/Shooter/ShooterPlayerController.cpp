// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Blueprint/UserWidget.h"

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ToSpawnPlayerOverlay)
	{
		const auto OverlayWidget = CreateWidget(this, ToSpawnPlayerOverlay);
		if (OverlayWidget)
		{
			OverlayWidget->AddToViewport();
		}
	}
}
