// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UShooterAnimInstance::UShooterAnimInstance():
	ShooterCharacter(nullptr),
	Speed(0.f),
	bIsInAir(false),
	bIsAcceleration(false),
	MovementOffsetYaw(0.f),
	LastMovementOffsetYaw(0.f),
	bAiming(false),
	bCharacterIsReloading(false),
	CharacterYaw(0.f),
	LastCharacterYaw(0.f),
	RootYawOffset(0.f),
	ControllerPitch(0.f)
{
}

void UShooterAnimInstance::InPlace(AShooterCharacter* Char)
{
	if (!Char) return;
	if (Speed > 0)
	{
		//we dont want turn when we moving
		ResetTurnInPlaceVariables(Char);
	}
	else
	{
		bCharacterIsReloading = Char->GetECombatState() == ECombatState::ECS_Reloading;
		ControllerPitch = Char->GetBaseAimRotation().Pitch;
		LastCharacterYaw = CharacterYaw;
		CharacterYaw = Char->GetActorRotation().Yaw;
		const float Delta = (CharacterYaw - LastCharacterYaw);

		// root yaw offset, update and claped to {180, -180}
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - Delta);

		const float IsTurnInPlaceAnimationPlaying{ GetCurveValue(TEXT("IsAnimationPlaying")) };
		TurnInPlaceAnimationPlaying(IsTurnInPlaceAnimationPlaying);
	}
}

void UShooterAnimInstance::TurnInPlaceAnimationPlaying(const float& IsTurnInPlaceAnimationPlaying)
{
	if (IsTurnInPlaceAnimationPlaying > 0)
	{
		LastRotationCurve = RotationCurve;
		RotationCurve = GetCurveValue(TEXT("Rotation"));
		const float DeltaCurve{ RotationCurve - LastRotationCurve };
		RootYawOffset > 0 ? RootYawOffset -= DeltaCurve : RootYawOffset += DeltaCurve;

		const float ABSRootYawOffset{ FMath::Abs(RootYawOffset) };
		if (ABSRootYawOffset > 90.f)
		{
			const float YawExcess{ ABSRootYawOffset - 90.f };
			RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
		}
	}
}

void UShooterAnimInstance::ResetTurnInPlaceVariables(AShooterCharacter* Char)
{
	RootYawOffset = 0.f;
	CharacterYaw = Char->GetActorRotation().Yaw;
	LastCharacterYaw = CharacterYaw;
	RotationCurve = 0.f;
	LastRotationCurve = 0.f;
}

void  UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (ShooterCharacter == nullptr)
	{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
	if (!ShooterCharacter) return;
	}
	
	//get speed our char
	FVector Velocity = ShooterCharacter->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	//our char IsInAir?
	bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

	//our char Acceleration? change bool bIsAcceleration
	(ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0) ? bIsAcceleration = true : bIsAcceleration = false;

	FRotator AimRotation = ShooterCharacter->GetBaseAimRotation(); //get base offset our aim
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());//get offset our movement
	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation,AimRotation).Yaw;

	if (ShooterCharacter->GetVelocity().Size() > 0.f)
	{
		LastMovementOffsetYaw = MovementOffsetYaw ;
	}
	bAiming = ShooterCharacter->GetAimingCondition();
	
	InPlace(ShooterCharacter);
}


void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
