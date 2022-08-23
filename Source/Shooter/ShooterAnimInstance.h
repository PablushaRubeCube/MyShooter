// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	UShooterAnimInstance();

private://variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	class AShooterCharacter* ShooterCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAcceleration;

	//Offset yaw used for strafing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	//last offset yaw before we stop our char
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crouch", meta = (AllowPrivateAccess = "true"))
	bool bIsCrouch;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	bool bCharacterIsReloading;

	float CharacterYaw;

	float LastCharacterYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ControllerPitch;

	/** Rotation curve value this frame*/
	float RotationCurve;
	/** Rotation curve value last frame*/
	float LastRotationCurve;

	FRotator CharacterRotator;
	FRotator LastCharacterRotator;
	UPROPERTY(BlueprintReadOnly, Category = "Lean", meta = (AllowPrivateAccess = "true"))
	float YawDelta;

	/** Set Blend Weight to ShootAnimation*/
	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BlendWeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crouch", meta = (AllowPrivateAccess = "true"))
	bool bIsTurnInPlaceAnimPlaying;

private://functions

	void InPlace(AShooterCharacter* Char);

	void TurnInPlaceAnimationPlaying(const float& IsTurnInPlaceAnimationPlaying);

	//Reset 
	void ResetTurnInPlaceVariables(AShooterCharacter* Char);

	void Lean(AShooterCharacter* Char, float DeltaTime);

	void SetBlendWeight();

	void SetMovementOffsetYaw();

	bool IsCharacterValid();

public:
	
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;
};
