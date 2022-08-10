// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

private://variables
	UPROPERTY(EditDefaultsOnly, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> ToSpawnPlayerOverlay;

protected://functions

	virtual void BeginPlay() override;

};
