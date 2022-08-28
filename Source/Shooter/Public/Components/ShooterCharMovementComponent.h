// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterCharMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:

	UShooterCharMovementComponent();

private://variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float AimMultiplier;


public://functions

	virtual float GetMaxSpeed() const override;

};
