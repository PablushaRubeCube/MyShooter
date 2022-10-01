// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


UShooterAnimInstance::UShooterAnimInstance()
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
		bIsTurnInPlaceAnimPlaying = true;
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
	else
	{
		bIsTurnInPlaceAnimPlaying = false;
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

void UShooterAnimInstance::Lean(AShooterCharacter* Char, float DeltaTime)
{
	LastCharacterRotator = CharacterRotator;
	CharacterRotator = Char->GetActorRotation();

	const FRotator DeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotator, LastCharacterRotator);

	const float Target = (DeltaRotator.Yaw / DeltaTime);
	const float Interp = FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.f);
	YawDelta = FMath::Clamp(Interp, -90.f, 90.f);
}

void UShooterAnimInstance::SetBlendWeight()
{
	if (bIsCrouch && !bIsTurnInPlaceAnimPlaying && !bCharacterIsReloading)
	{
		BlendWeight = 0.2f;
	}
	else if (bIsTurnInPlaceAnimPlaying)
	{
		BlendWeight = 0.f;
	}
	else
	{
		BlendWeight = 1.f;
	}
}

bool UShooterAnimInstance::IsCharacterValid()
{
	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
		if (!ShooterCharacter) return false;
	}
	return true;
}

void UShooterAnimInstance::SetMovementOffsetYaw()
{
	//our char Acceleration? change bool bIsAcceleration
	(ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0) ? bIsAcceleration = true : bIsAcceleration = false;

	FRotator AimRotation = ShooterCharacter->GetBaseAimRotation(); //get base offset our aim
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());//get offset our movement
	MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

	if (ShooterCharacter->GetVelocity().Size() > 0.f)
	{
		LastMovementOffsetYaw = MovementOffsetYaw;
	}
}

void  UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if(!IsCharacterValid()) return;
	
	//get speed our char
	FVector Velocity = ShooterCharacter->GetVelocity();
	Velocity.Z = 0;
	Speed = Velocity.Size();

	bIsCrouch = ShooterCharacter->bIsCrouched;
	//our char IsInAir?
	bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();
	bAiming = ShooterCharacter->GetAimingCondition();

	SetMovementOffsetYaw();
	InPlace(ShooterCharacter);
	Lean(ShooterCharacter, DeltaTime);
	SetBlendWeight();
}

void UShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
