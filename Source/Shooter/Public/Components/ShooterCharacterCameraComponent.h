// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "ShooterCharacterCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterCharacterCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public://variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MakeEditWidget = true))
	TArray<FVector> PickupAmmoLocation;
	
};
