// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShooterCharMovementComponent.h"
#include "../ShooterCharacter.h"

UShooterCharMovementComponent::UShooterCharMovementComponent()
{
	//movement settings
	MaxAcceleration = 1024.f;
	GroundFriction = 2.f;
	BrakingDecelerationWalking = 500.f;
	JumpZVelocity = 420.f;
	RotationRate = FRotator(0.f, 540.f, 0.f);//... at this rotation rate
	bOrientRotationToMovement = false;// charcater moves in the direction of input...
	AirControl = 0, 05;
	NavAgentProps.bCanCrouch = true;
	bCanWalkOffLedgesWhenCrouching = true;

	AimMultiplier = 0.5f;
}

float UShooterCharMovementComponent::GetMaxSpeed() const
{
	const float Speed = Super::GetMaxSpeed();
	const auto Char = Cast<AShooterCharacter>(GetPawnOwner());
	return Char && Char->GetAimingCondition() ? AimMultiplier * Speed : Speed;
}

