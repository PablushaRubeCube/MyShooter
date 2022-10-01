// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "Item.h"
#include "Weapon.h"
#include "Items/Ammo/Ammo.h"
#include "Components/ShooterCharacterCameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/ShooterCharMovementComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/PickupComponent.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacter, All, All)

// Sets default values
AShooterCharacter::AShooterCharacter(const FObjectInitializer& ObjInit) :
	Super (ObjInit.SetDefaultSubobjectClass<UShooterCharMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create a camera boom (pills in towards the character if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.f;// the camera follows at tis distance behind the character
	CameraBoom->bUsePawnControlRotation = true;// Rotate arm based on the controller
	CameraBoom->SocketOffset = FVector (0.f, 50.f, 70.f);

	FollowCamera = CreateDefaultSubobject<UShooterCharacterCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));

	PickupComponent = CreateDefaultSubobject <UPickupComponent>(TEXT("PickupComponent"));

	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandComponent"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	//Disable controller rotate for mesh. let the controller onlu affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(FollowCamera);
	check(HandSceneComponent);
	check(GetMovementComponent());
	check(CameraBoom);
	check(WeaponComponent);
	check(InventoryComponent);

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
	
//	FVector Vector = (FollowCamera->GetComponentLocation() + (FollowCamera->GetForwardVector() * FollowCamera->PickupAmmoLocation[0]));
//	UE_LOG(LogCharacter, Display, TEXT("The vector value is: %s"), *Vector.ToString());

	//Handle Interp Zoom when we aim
	InterpZoomFOV(DeltaTime);

	//Change turn and lookUp Rate for Gamepad
	SetTurnLookRate();

	CalculateCrosshireSpead(DeltaTime);
		
	TraceForItem();
	
}

void AShooterCharacter::TraceForItem()
{
	if (bOverlapAgroItem)
	{
	FHitResult PickUpItem;
	FVector TempVector;
	LineTracePickUp(PickUpItem, TempVector);
		if (PickUpItem.bBlockingHit)
		{
			TraceHitItem = Cast <AItem>(PickUpItem.Actor);
			if(TraceHitItem && TraceHitItem->GetItemStates() == EItemStates::EIS_EquipInterping)
			{
				TraceHitItem = nullptr;
			}
			if (TraceHitItem && TraceHitItem->GetPickUpWidget())
			{
				TraceHitItem->GetPickUpWidget()->SetVisibility(true);
			}
			if (TraceHitItemLast) // if we have last frame
			{
				if (TraceHitItemLast != TraceHitItem)
				{
					//if lastFrame diffirent  Item//or lastFrame null
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

bool AShooterCharacter::LineTracePickUp(FHitResult& PickUpitem, FVector& OutVector)
{
	//Get current size of the viewport
	FVector2D ViewportSize = FVector2D::ZeroVector;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	//Get screen space location of crosshairs
	FVector2D CrosshairLocation(ViewportSize.X / 2, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition = FVector::ZeroVector;
	FVector CrosshairWorldDirection = FVector::ZeroVector;

	// Get world position and direction of crosshairs
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);
	if (bScreenToWorld)// was derpojection successful?
	{
		const FVector StartPoint = CrosshairWorldPosition;
		const FVector EndPoint = (CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f);
		OutVector = EndPoint;
		//trace outward from crosshairs world location
		if (!GetWorld())return false;
		GetWorld()->LineTraceSingleByChannel(PickUpitem, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility);

		if (PickUpitem.bBlockingHit)// was there a trace hit?
		{
			return true;
		}
	}
	return false;
}

FVector AShooterCharacter::GetInterpLocation(const AItem* Item)
{
	const auto Ammo = Cast<AAmmo>(Item);
	if (Ammo)
	{
		const FVector CameraLocation{ FollowCamera->GetComponentLocation() };
		const FVector CameraForwardLocation{ FollowCamera->GetForwardVector() };
		return CameraLocation + (CameraForwardLocation * FollowCamera->PickupAmmoLocation[Ammo->GetIndexInterpLocation()]);
	}
	return GetCameraInterpLocation();
}

int32 AShooterCharacter::GetInterpIndexLocation()
{
	if (++CurrentAmmoLocationIndex < FollowCamera->PickupAmmoLocation.Num())
	{
		return CurrentAmmoLocationIndex;
	}
	else
	{
		CurrentAmmoLocationIndex = 0;
		return CurrentAmmoLocationIndex;
	}
}

void AShooterCharacter::SelectButtonPressed()
{
	if (TraceHitItem)
	{
		auto Item = Cast<AItem>(TraceHitItem);
		if (!Item) return;
		Item->StartPickupItem(this);
		TraceHitItem = nullptr;
		TraceHitItemLast = nullptr;
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
		UnCrouch();	
	}
	else
	{
		Crouch();
	}	
}

void AShooterCharacter::GetPickupItem(AItem* Item)
{
	if (!Item) return;
	Item->SetOwner(this);
	Item->GetPickupItem();
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

	const TMap<FName, int32> PressedItems =
	{ {"DefaultItem", 0},{"FirstItem", 1} ,{"SecondItem", 2} ,{"ThirdItem", 3} ,{"FourItem", 4} ,{"FiveItem", 5} };
	DECLARE_DELEGATE_OneParam(FChooseItem, int32);
	for (const TTuple<FName, int32>& Element : PressedItems)
	{
	PlayerInputComponent->BindAction<FChooseItem>(Element.Key, IE_Pressed, InventoryComponent, &UInventoryComponent::ChooseInventoryItem, Element.Value);
	}
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
