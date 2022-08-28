// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"


UENUM(BlueprintType)
enum class ECombatState : uint8
{
ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
ECS_FireTimerProgress UMETA(DisplayName = "FireInProgress"),
ECS_Reloading UMETA(DisplayName = "Reloading"),

ECS_MAX UMETA(DisplayName = "Default")
};

UCLASS()
class SHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter(const FObjectInitializer& ObjInit);

private://functions

//	void DisableCameraLag();

	class UShooterCharMovementComponent* qweqwe;

protected://functions
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

	//change zoom camera
	void InterpZoomFOV(float DeltaTime);

	//Change rate for gamepad
	void SetTurnLookRate();

	//Change Crosshire Spread when we do anythink
	void CalculateCrosshireSpead(float DeltaTime);

	//call in eventtick for display Widget of item class
	void DisplayWidget();

	//Select Button Functions
	void SelectButtonPressed();
	void SelectButtonReleased();

	void CrouchButtonPressed();

	void ToogleCrouch(bool CharacterIsCrouched);

private://variables

	//Camera boom positioning the camera behind the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	//CameraComponent Attach to SpringArm socket
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class UWeaponComponent* WeaponComponent;

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

	//true if we overlap any ItemClass
	bool bOverlapAgroItem;

	//increment Count if we Overlap anyItemClass
	int8 AgroCountItem;

	//Need for toogle display widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	class AItem* TraceHitItemLast;

	//Ref To What we look right now
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta= (AllowPrivateAccess = "true"))
	float CameraInterpDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float CameraInterpElevation;

	/** Combat State, can only fire or reload */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	//Clip Attach for this SceneComponent when we reload clip
	UPROPERTY()
	USceneComponent* HandSceneComponent;

public://functions	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Returns CameraBoom subobject
	FORCEINLINE  USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//Return CameraComponent subobject
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//Return HandComponent for clip reload function
	UFUNCTION(BlueprintPure)
	USceneComponent* GetHandSceneComponent() const { return HandSceneComponent; }

	void IncrementAgroCountItem(int8 Amount);

	FVector GetCameraInterpLocation();

	void GetPickupItem(class AItem* Item);

	FORCEINLINE ECombatState GetECombatState() const { return CombatState; }

	FORCEINLINE void SetECombatState(ECombatState State) { CombatState = State; }

	bool GetAimingCondition();
};
