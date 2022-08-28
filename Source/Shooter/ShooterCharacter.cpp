// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Item.h"
#include "Components/WidgetComponent.h"
#include "Weapon.h"
#include "Components/ShooterCharMovementComponent.h"
#include "Components/WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacter, All, All)

// Sets default values
AShooterCharacter::AShooterCharacter(const FObjectInitializer& ObjInit) :
	Super (ObjInit.SetDefaultSubobjectClass<UShooterCharMovementComponent>(ACharacter::CharacterMovementComponentName)),
	//Default rate, overide this it tick function
	BaseTurnAtRate(100.f),
	BaseLookUpAtRate(100.f),
	//Mouse Sensetivity
	MouseHipTurn(1.f),
	MouseHipLookUp(1.f),
	MouseAimTurn(0.2f),
	MouseAimLookUp(0.2f),
	//Rate when we dont aim
	HipTurnAtRate(90.f),
	HipLookUpAtRate(90.f),
	//Rate when we aiming
	AimTurnAtRate(20.f),
	AimLookUpAtRate(20.f),
	//Camera settings when we aiming
	DefaultCameraFOV(0.f), //set in begin play
	AimCameraFOV(40.f),
	CurrentCameraFOV(0.f), //set in begin play
	InterpZoomSpeed(20.f),
	//Settings Crosshire
	CrosshiresAimFactor(0.f),
	CrosshiresIsInAirFactor(0.f),
	CrosshiresMultiplier(0.f),
	CrosshiresVelocityFactor(0.f),
	CrosshiresShootingFactor(0.f),
	//variables for count Agro item
	bOverlapAgroItem(false),
	AgroCountItem(0.f),
	//properties for Items CameraLocation
	CameraInterpDistance(250.f),
	CameraInterpElevation(65.f),
	//Combat Var
	CombatState(ECombatState::ECS_Unoccupied)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create a camera boom (pills in towards the character if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;// the camera follows at tis distance behind the character
	CameraBoom->bUsePawnControlRotation = true;// Rotate arm based on the controller
	CameraBoom->SocketOffset = FVector (0.f, 50.f, 70.f);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));

	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandComponent"));

	//Disable controller rotate for mesh. let the controller onlu affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;
}

//void AShooterCharacter::DisableCameraLag()
//{
//	CameraBoom->bEnableCameraLag = (false);
//}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(FollowCamera);
	check(HandSceneComponent);
	check(GetMovementComponent());
	check(CameraBoom);
	check(WeaponComponent);

	qweqwe = Cast<UShooterCharMovementComponent>(GetMovementComponent());

	if (FollowCamera)
	{
		DefaultCameraFOV = FollowCamera->FieldOfView;
		CurrentCameraFOV = DefaultCameraFOV;
	}

}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Handle Interp Zoom when we aim
	InterpZoomFOV(DeltaTime);

	//Change turn and lookUp Rate for Gamepad
	SetTurnLookRate();

	CalculateCrosshireSpead(DeltaTime);
		
	DisplayWidget();
	
}

void AShooterCharacter::DisplayWidget()
{
	if (bOverlapAgroItem)
	{
	FHitResult PickUpItem;
	FVector TempVector;
	WeaponComponent->ToogleVisibilityWidgetPickUp(PickUpItem, TempVector);
		if (PickUpItem.bBlockingHit)
		{
			TraceHitItem = Cast <AItem>(PickUpItem.Actor);
			if (TraceHitItem && TraceHitItem->GetPickUpWidget())
			{
				TraceHitItem->GetPickUpWidget()->SetVisibility(true);
			}
			if (TraceHitItemLast) // if we have last frame
			{
				if (TraceHitItemLast != TraceHitItem)
				{
					//UE_LOG(LogTemp, Warning, TEXT("TET"));
					//if lastFrame diffirent  Item
					//or lastFrame null
					TraceHitItemLast->GetPickUpWidget()->SetVisibility(false);
				}
			}
			TraceHitItemLast = TraceHitItem; // save last frame item
		}
	}
	//if we end overlap sphere
	else if (TraceHitItemLast)
	{
		TraceHitItemLast->GetPickUpWidget()->SetVisibility(false);
	}
}

void AShooterCharacter::SelectButtonPressed()
{
	if (TraceHitItem)
	{
		auto Weapon = Cast<AWeapon>(TraceHitItem);
		if (!Weapon) return;
		Weapon->StartCurveItem(this);
		//SwapWeapon(Weapon);
		TraceHitItem = nullptr;
		TraceHitItemLast = nullptr;

		if (Weapon->GetPickupSound())
		UGameplayStatics::PlaySound2D(this, Weapon->GetPickupSound());
	}
}

void AShooterCharacter::SelectButtonReleased()
{
}

void AShooterCharacter::CrouchButtonPressed()
{
	ToogleCrouch(bIsCrouched);
}

void AShooterCharacter::ToogleCrouch(bool CharacterIsCrouched)
{
	if (CharacterIsCrouched)
	{
		FTimerHandle CameraLagTimerHandle;
	//	GetWorldTimerManager().SetTimer(CameraLagTimerHandle, this, &AShooterCharacter::DisableCameraLag, 1.f);
		UnCrouch();	
	}
	else
	{
		//CameraBoom->bEnableCameraLag = (true);
		Crouch();
	}	
}

void AShooterCharacter::GetPickupItem(AItem* Item)
{
	AWeapon* Weapon = Cast<AWeapon>(Item);
	if (Weapon)
	{
		if (Weapon->GetEquipSound())
		UGameplayStatics::PlaySound2D(this, Weapon->GetEquipSound());

		WeaponComponent->SwapWeapon(Weapon);
	}
}

bool AShooterCharacter::GetAimingCondition()
{
	return WeaponComponent->GetAimingCondition();
}

void AShooterCharacter::CalculateCrosshireSpead(float DeltaTime)
{
	FVector2D WalkSpeedRange{ 0.f, 600.f };
	FVector2D VelocityMultiplierRange{ 0.f, 1.f };
	FVector Velocity{ GetVelocity() };
	Velocity.Z = 0.f;

	//calculate spread when we do Velocity
	CrosshiresVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

	//Calculate spread when we in Air
	if (GetCharacterMovement()->IsFalling())
	{
		CrosshiresIsInAirFactor = FMath::FInterpTo(CrosshiresIsInAirFactor, 2.25f, DeltaTime, 2.25f);
	}
	else
	{
		CrosshiresIsInAirFactor = FMath::FInterpTo(CrosshiresIsInAirFactor, 0.f, DeltaTime, 30.f);
	}

	//Calculate spread when we shooting
	if (WeaponComponent->GetbShooting())
	{
		CrosshiresShootingFactor = FMath::FInterpTo(CrosshiresShootingFactor, 0.3f, DeltaTime, 60.f);
	}
	else
	{
		CrosshiresShootingFactor = FMath::FInterpTo(CrosshiresShootingFactor, 0.f, DeltaTime, 60.f);
	}

	//Calculate spread when we Aiming
	if (WeaponComponent->GetbShooting())
	{
		CrosshiresAimFactor = FMath::FInterpTo(CrosshiresAimFactor, -0.5f, DeltaTime, 30.f);
	}
	else
	{
		CrosshiresAimFactor = FMath::FInterpTo(CrosshiresAimFactor, 0.f, DeltaTime, 10.f);
	}
	CrosshiresMultiplier = 0.5f + CrosshiresVelocityFactor + CrosshiresIsInAirFactor + CrosshiresAimFactor + CrosshiresShootingFactor;
}



void AShooterCharacter::SetTurnLookRate()
{
	if (WeaponComponent->GetAimingCondition())
	{
		BaseTurnAtRate = AimTurnAtRate;
		BaseLookUpAtRate = AimLookUpAtRate;
	}
	else
	{
		BaseTurnAtRate = HipTurnAtRate;
		BaseLookUpAtRate = HipLookUpAtRate;
	}
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpAtRate);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShooterCharacter::MouseTurn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::MouseLookUp);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed,this,  &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction(TEXT("FireButton"), IE_Pressed, WeaponComponent, &UWeaponComponent::FireButtonPressed);
	PlayerInputComponent->BindAction(TEXT("FireButton"), IE_Released, WeaponComponent, &UWeaponComponent::FireButtonReleased);

	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, WeaponComponent, &UWeaponComponent::AimButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, WeaponComponent, &UWeaponComponent::AimButtomReleased);

	PlayerInputComponent->BindAction(TEXT("Select"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SelectButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Select"), EInputEvent::IE_Released, this, &AShooterCharacter::SelectButtonReleased);

	PlayerInputComponent->BindAction(TEXT("ReloadButton"), EInputEvent::IE_Pressed, WeaponComponent, &UWeaponComponent::ReloadButtonPressed);
	
	PlayerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &AShooterCharacter::CrouchButtonPressed);
}

void AShooterCharacter::IncrementAgroCountItem(int8 Amount)
{ 
	if (AgroCountItem + Amount <= 0)
	{
		AgroCountItem = 0;
		bOverlapAgroItem = false;
	}
	else
	{
		AgroCountItem += Amount;
		bOverlapAgroItem = true;
	}
}

FVector AShooterCharacter::GetCameraInterpLocation()
{

	const FVector CameraLocation{ FollowCamera->GetComponentLocation() };
	const FVector CameraForwardLocation{ FollowCamera->GetForwardVector() };

	return CameraLocation + CameraForwardLocation *CameraInterpDistance + FVector(0.f,0.f, CameraInterpElevation);
}

void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0))
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator RotationYaw(0.f, Rotation.Yaw, 0.f);
		FVector Direction(FRotationMatrix(RotationYaw).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0))
	{
		FRotator Rotation = Controller->GetControlRotation();
		FRotator RotationYaw(0.f, Rotation.Yaw, 0.f);
		FVector Direction(FRotationMatrix(RotationYaw).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnAtRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseTurnAtRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::MouseTurn(float Value)
{
	float ScaleFactor;
	if (WeaponComponent->GetAimingCondition())
	{
		ScaleFactor = MouseAimTurn;
	}
	else
	{
		ScaleFactor = MouseHipTurn;
	}
	AddControllerYawInput(Value * ScaleFactor);
}

void AShooterCharacter::MouseLookUp(float Value)
{
	float ScaleFactor;
	if (WeaponComponent->GetAimingCondition())
	{
		ScaleFactor = MouseAimLookUp;
	}
	else
	{
		ScaleFactor = MouseHipLookUp;
	}
	AddControllerPitchInput(Value * ScaleFactor);
}

void AShooterCharacter::InterpZoomFOV(float DeltaTime)
{
	if (WeaponComponent->GetAimingCondition())
	{
		CurrentCameraFOV = FMath::FInterpTo(CurrentCameraFOV, AimCameraFOV, DeltaTime, InterpZoomSpeed);
	}
	else
	{
		CurrentCameraFOV = FMath::FInterpTo(CurrentCameraFOV, DefaultCameraFOV, DeltaTime, InterpZoomSpeed);
	}
	FollowCamera->SetFieldOfView(CurrentCameraFOV);
}
