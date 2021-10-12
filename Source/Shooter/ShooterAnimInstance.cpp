// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void  UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (ShooterCharacter == nullptr)
	{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	}
	if (ShooterCharacter)
	{
		//get speed our char
		FVector Velocity = ShooterCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		//our char IsInAir?
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

		//our char Acceleration? change bool bIsAcceleration
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
		{
			bIsAcceleration = true;
		}
		else
		{
			bIsAcceleration = false;
		}
		FRotator AimRotation = ShooterCharacter->GetBaseAimRotation(); //get base offset our aim
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());//get offset our movement
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation,AimRotation).Yaw;

		if (ShooterCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw ;
		}
		bAiming = ShooterCharacter->GetAimingCondition();
	}
}


void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
