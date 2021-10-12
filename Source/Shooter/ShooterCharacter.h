// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//functions to move our char
	void MoveForward(float Value);
	void MoveRight(float Value);

	//functions to look around Gamepad
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);

	//function to look around Mouse
	void MouseTurn(float Value);
	void MouseLookUp(float Value);

	//Open fire when we pressed fireButton
	void FireWeapon();

	//Create and return BeamLocation for particles shots 
	bool GetBeamEndLocation(const FVector& SocketBeam, FVector& OutBeamLocation);

	//change camera when we aim 
	void AimButtonPressed();
	void AimButtomReleased();

	//change zoom camera
	void InterpZoomFOV(float DeltaTime);

	//Change rate for gamepad
	void SetTurnLookRate();

	//Change Crosshire Spread when we do anythink
	void CalculateCrosshireSpead(float DeltaTime);

	//Change Crosshire Spread when we shoting
	void StartCrosshiresFire();
	UFUNCTION()
	void EndCrosshiresFire();


	//Call when we pressed LMB
	void FireButtonPressed();
	void FireButtonReleased();

	//Timer For AutoShooting
	void StartAutoFire();
	UFUNCTION()
	void ResetAutoShooting();

	//Call When we need Display Widget Item Class and When we Shoot. use 2 linetrace for that
	bool ToogleVisibilityWidgetPickUp(FHitResult &PickUpitem, FVector &OutVector);

	//call in eventtick for display Widget of item class
	void DisplayWidget();

	//Spawn Weapon when we start
	class AWeapon* SpawnDefaultWeapon();

	//EquipWeapon
	void EquipWeapon(AWeapon* EquipWeapon);

	//Drop Weapon
	void DropWeapon();

	//Select Button Functions
	void SelectButtonPressed();
	void SelectButtonReleased();

	//Swap Weapon
	void SwapWeapon(AWeapon* SwapWeapon);
	
private:

	//Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	//CameraComponent Attach to SpringArm socket
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	//BaseValue to turn our char
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnAtRate;

	//BaseValue to Lookup our char
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpAtRate;

	//HipValue to turn our char Mouse
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseHipTurn;

	//HipValue to Lookup our char Mouse
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseHipLookUp;

	//HipValue to turn our char Mouse
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseAimTurn;

	//HipValue to Lookup our char Mouse
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float MouseAimLookUp;

	//HipValue to turn our char Gamepad
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipTurnAtRate;

	//HipValue to Lookup our char Gamepad
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float HipLookUpAtRate;

	//HipValue to turn our char Gamepad
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimTurnAtRate;

	//HipValue to Lookup our char Gamepad
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimLookUpAtRate;

	//sound cue for gun shots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireShotSound;

	//particle effect for gun fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	//particle effect in shotend
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	//beam smoke
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;

	//combat montage for shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;

	//true when we aim
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	//fov for start
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float DefaultCameraFOV;

	//fov when we aim
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float AimCameraFOV;

	//camera at a specific time
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float CurrentCameraFOV;

	//speed of zoom camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float InterpZoomSpeed;

	//crosshires Lines

	//Base crosshires value
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshire, meta = (AllowPrivateAccess = "true"))
	float CrosshiresMultiplier;

	//crosshires value when we run
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshire, meta = (AllowPrivateAccess = "true"))
	float CrosshiresVelocityFactor;

	//crosshires value when we Aim
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshire, meta = (AllowPrivateAccess = "true"))
	float  CrosshiresAimFactor;

	//crosshires value when we Jump
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshire, meta = (AllowPrivateAccess = "true"))
	float  CrosshiresIsInAirFactor;

	//crosshires value when we shoot
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshire, meta = (AllowPrivateAccess = "true"))
	float  CrosshiresShootingFactor;

	//Crosshair 
	FTimerHandle CrosshiresTimer;
	bool bShooting;
	float CrosshiresDelayTimer;

	//Variables for AutoShooting
	bool bFireButtonPressed;
	bool bShouldFire;
	float ShootingRate;
	FTimerHandle ShootingTimer;

	//true if we overlap any ItemClass
	bool bOverlapAgroItem;

	//increment Count if we Overlap anyItemClass
	int8 AgroCountItem;

	//Need for toogle display widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItemLast;

	//EquippedWeapon our Char
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon;

	//Chose Weapon for Spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class TSubclassOf<AWeapon> DefaultWeapon;

	//Ref To What we look right now
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta= (AllowPrivateAccess = "true"))
	float CameraInterpDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float CameraInterpElevation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Returns CameraBoom subobject
	FORCEINLINE  USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//Return CameraComponent subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//Return Aiming condition our char
	FORCEINLINE bool GetAimingCondition() const { return bAiming; }

	void IncrementAgroCountItem(int8 Amount);

	FVector GetCameraInterpLocation();
};
