// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"
#include "Item.h"
#include "Components/WidgetComponent.h"
#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCharacter, All, All)

// Sets default values
AShooterCharacter::AShooterCharacter() :
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
	bAiming(false),
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
	bShooting(false),
	CrosshiresDelayTimer(0.05f),
	//AutoShooting Valiables
	bFireButtonPressed(false),
	bShouldFire(true),
	ShootingRate(0.1f),
	//variables for count Agro item
	bOverlapAgroItem(false),
	AgroCountItem(0.f),
	//properties for Items CameraLocation
	CameraInterpDistance(250.f),
	CameraInterpElevation(65.f),
	//Start Ammo value
	Init9mmAmmo(9),
	InitARAmmo(30),
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

	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandComponent"));

	//Disable controller rotate for mesh. let the controller onlu affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = true;

	//Congigure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false;// charcater moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);//... at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(FollowCamera);
	check(HandSceneComponent);

	if (FollowCamera)
	{
		DefaultCameraFOV = FollowCamera->FieldOfView;
		CurrentCameraFOV = DefaultCameraFOV;
	}

	InitAmmo();
	//spawn default weapon and Equip Weapon
	EquipWeapon(SpawnDefaultWeapon());

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
	ToogleVisibilityWidgetPickUp(PickUpItem, TempVector);
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

//spawn weapon when we start
AWeapon* AShooterCharacter::SpawnDefaultWeapon()
{
	if (DefaultWeapon) //if we have one
	{
		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeapon);//spawn weapon what we chose
		return SpawnedWeapon;
	}
	return nullptr;
}

void AShooterCharacter::EquipWeapon(AWeapon* EquipWeapon)
{
	if (!EquipWeapon) return;
	EquipWeapon->GetCollisionBox()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EquipWeapon->GetAgroSphere()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (EquipWeapon)
	{
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("hand_rSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(EquipWeapon, GetMesh());//...get socket and attach weapon for him
		}
		EquippedWeapon = EquipWeapon; // set equippedWEapon
		EquippedWeapon->SetItemStates(EItemStates::EIS_Equipped);//Change Item State
	}
}

void AShooterCharacter::DropWeapon()
{
	if (EquippedWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedWeapon->GetSkeletalMeshComponent()->DetachFromComponent(DetachmentTransformRules);

		EquippedWeapon->SetItemStates(EItemStates::EIS_Falling);
		EquippedWeapon->ThrowWeapon();
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

void AShooterCharacter::SwapWeapon(AWeapon* SwapWeapon)
{
	DropWeapon();
	EquipWeapon(SwapWeapon);
}

void AShooterCharacter::InitAmmo()
{
	AmmoCharacter.Add(EAmmoType::EAT_9MM, Init9mmAmmo);
	AmmoCharacter.Add(EAmmoType::EAT_AR, InitARAmmo);

}

bool AShooterCharacter::IsAmmoEmpty()
{
	if (!EquippedWeapon) return true;
	return EquippedWeapon->GetAmmo() <= 0;
}

void AShooterCharacter::PlayFireSound()
{
	//play fire sound
	if (FireShotSound)
	{
		UGameplayStatics::PlaySound2D(this, FireShotSound);
	}
}

void AShooterCharacter::SendBullet()
{
//send bullet
const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->GetSkeletalMeshComponent()->GetSocketByName(TEXT("BarrelSocket"));//get socket weapon

	if (BarrelSocket)//..we have socket for shoot?
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetSkeletalMeshComponent());//get socket transform
		if (MuzzleFlash)// if we have ShotParticles->Spawn
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}
		//Update BeamEndPoint for Particles
		FVector BeamEndPoint;
		bool bBeamLocation = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEndPoint);
		if (bBeamLocation)
		{
			//spawn impactParticle after update BeamEnd
			if (ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, BeamEndPoint);
			}
			if (BeamParticles)
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticles, SocketTransform);
				Beam->SetVectorParameter("Target", BeamEndPoint);
			}
		}
		//Change Spread bool bShooting and start timer
		StartCrosshiresFire();
	}
}

void AShooterCharacter::PlayGunFireMontage()
{
	//AnimMontage for HipFireAnimation 
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("Start_Fire"));
	}
}

void AShooterCharacter::ReloadButtonPressed()
{
	ReloadWeapon();
}

void AShooterCharacter::ReloadWeapon()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (!EquippedWeapon || EquippedWeapon->IsMagazineFull()) return;

	if (CarryingAmmo())
	{
		const auto AnimInstance = GetMesh()->GetAnimInstance();
		if (ReloadMontage && AnimInstance)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(EquippedWeapon->GetMontageWeaponSectionName());
			CombatState = ECombatState::ECS_Reloading;
		}
	}
}

void AShooterCharacter::FinishReloading()
{
	CombatState = ECombatState::ECS_Unoccupied;

	if (!EquippedWeapon) return;
	const auto AmmoType = EquippedWeapon->GetAmmoType();
	if (!AmmoCharacter.Contains(AmmoType)) return;
	

	const int32 EmptySpaceMagazine = EquippedWeapon->GetMagazineCapacity() - EquippedWeapon->GetAmmo();
	if (EmptySpaceMagazine > AmmoCharacter[AmmoType])
	{
		//Take all Character ammo for magazine
		EquippedWeapon->ReloadMagazine(AmmoCharacter[AmmoType]);
		AmmoCharacter.Add(AmmoType, 0 );
	}
	else
	{
		// Take only what we need bullet
		EquippedWeapon->ReloadMagazine(EmptySpaceMagazine);
		const int32 BulletSpend =  AmmoCharacter[AmmoType] - EmptySpaceMagazine;
		AmmoCharacter.Add(AmmoType, BulletSpend);
	}
}

bool AShooterCharacter::CarryingAmmo()
{
	if (!EquippedWeapon) return false;
	const auto WeaponAmmoType = EquippedWeapon->GetAmmoType();
	if (AmmoCharacter.Contains(WeaponAmmoType))
	{
		return AmmoCharacter[WeaponAmmoType] > 0;
	}
	return false;
}

void AShooterCharacter::TakeClip()
{
	if (!EquippedWeapon || !HandSceneComponent) return;
	const int32 BoneIndexClip = EquippedWeapon->GetSkeletalMeshComponent()->GetBoneIndex(EquippedWeapon->GetClipBoneName());
	const FTransform BoneTransformClip = EquippedWeapon->GetSkeletalMeshComponent()->GetBoneTransform(BoneIndexClip);

	FAttachmentTransformRules AttachmentRules (EAttachmentRule::KeepRelative, true);
	HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("Hand_L")));
	HandSceneComponent->SetWorldTransform(BoneTransformClip);

	EquippedWeapon->SetIsClipTaked(true);
}

void AShooterCharacter::ReturnClip()
{
	EquippedWeapon->SetIsClipTaked(false);
}

void AShooterCharacter::GetPickupItem(AItem* Item)
{
	AWeapon* Weapon = Cast<AWeapon>(Item);
	if (Weapon)
	{
		if (Weapon->GetEquipSound())
		UGameplayStatics::PlaySound2D(this, Weapon->GetEquipSound());

		SwapWeapon(Weapon);
	}
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
	if (bShooting)
	{
		CrosshiresShootingFactor = FMath::FInterpTo(CrosshiresShootingFactor, 0.3f, DeltaTime, 60.f);
	}
	else
	{
		CrosshiresShootingFactor = FMath::FInterpTo(CrosshiresShootingFactor, 0.f, DeltaTime, 60.f);
	}

	//Calculate spread when we Aiming
	if (bAiming)
	{
		CrosshiresAimFactor = FMath::FInterpTo(CrosshiresAimFactor, -0.5f, DeltaTime, 30.f);
	}
	else
	{
		CrosshiresAimFactor = FMath::FInterpTo(CrosshiresAimFactor, 0.f, DeltaTime, 10.f);
	}
	CrosshiresMultiplier = 0.5f + CrosshiresVelocityFactor + CrosshiresIsInAirFactor + CrosshiresAimFactor + CrosshiresShootingFactor;
}

void AShooterCharacter::StartCrosshiresFire()
{
	bShooting = true;

	GetWorldTimerManager().SetTimer(CrosshiresTimer, this, &AShooterCharacter::EndCrosshiresFire, CrosshiresDelayTimer);
}

void AShooterCharacter::EndCrosshiresFire()
{
	bShooting = false;
}

void AShooterCharacter::FireButtonPressed()
{
	bFireButtonPressed = true;
	FireWeapon();
}

void AShooterCharacter::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void AShooterCharacter::StartAutoFire()
{
	CombatState = ECombatState::ECS_FireTimerProgress;

	GetWorldTimerManager().SetTimer(ShootingTimer, this, &AShooterCharacter::ResetAutoShooting, ShootingRate);
}

void AShooterCharacter::ResetAutoShooting()
{
	CombatState = ECombatState::ECS_Unoccupied;

	if (!IsAmmoEmpty())
	{
		if (bFireButtonPressed)
		{
			FireWeapon();
		}
	}
	else
	{
		ReloadWeapon();
	}
}

bool AShooterCharacter::ToogleVisibilityWidgetPickUp(FHitResult& PickUpitem, FVector& OutVector)
{
	//Get current size of the viewport
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	//Get screen space location of crosshairs
	FVector2D CrosshairLocation(ViewportSize.X / 2, ViewportSize.Y / 2.f);
	//CrosshairLocation.Y -= 50.f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Get world position and direction of crosshairs
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);
	if (bScreenToWorld)// was derpojection siccessful?
	{
		const FVector StartPoint = CrosshairWorldPosition;
		const FVector EndPoint = (CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f);
		OutVector = EndPoint;
		//trace outward from crosshairs world location
		GetWorld()->LineTraceSingleByChannel(PickUpitem, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility);

		if (PickUpitem.bBlockingHit)// was there a trace hit?
		{
			return true;
		}
	}
	return false;
}

void AShooterCharacter::SetTurnLookRate()
{
	if (bAiming)
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

	PlayerInputComponent->BindAction(TEXT("FireButton"), IE_Pressed, this, &AShooterCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction(TEXT("FireButton"), IE_Released, this, &AShooterCharacter::FireButtonReleased);

	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed,this, &AShooterCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &AShooterCharacter::AimButtomReleased);

	PlayerInputComponent->BindAction(TEXT("Select"), EInputEvent::IE_Pressed, this, &AShooterCharacter::SelectButtonPressed);
	PlayerInputComponent->BindAction(TEXT("Select"), EInputEvent::IE_Released, this, &AShooterCharacter::SelectButtonReleased);

	PlayerInputComponent->BindAction(TEXT("ReloadButton"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ReloadButtonPressed);
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
	if (bAiming)
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
	if (bAiming)
	{
		ScaleFactor = MouseAimLookUp;
	}
	else
	{
		ScaleFactor = MouseHipLookUp;
	}
	AddControllerPitchInput(Value * ScaleFactor);
}

void AShooterCharacter::FireWeapon()
{
	if (!EquippedWeapon) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if(!IsAmmoEmpty())
	{
		PlayFireSound();
		SendBullet();
		PlayGunFireMontage();
		//decrement bullet current weapon
		EquippedWeapon->DecrementAmmo();

		StartAutoFire();
	}
}

bool AShooterCharacter::GetBeamEndLocation(const FVector& SocketBeam, FVector& OutBeamLocation)
{
	//first linetrace from center of screen to target
	FHitResult BeamHitResult;
	bool bBeamHit = ToogleVisibilityWidgetPickUp(BeamHitResult, OutBeamLocation);
	if (bBeamHit)
	{
		if (BeamHitResult.bBlockingHit)
		{
			OutBeamLocation = BeamHitResult.Location;////Set variablie "OutBeamLocation" if we hit something
		}
		else
		{
			//Set variablie "OutBeamLocation" in function "ToogleVisibilityWidgetPickUp" if we did we hit nothing
		}
	}
	//Second LineTrace From barrel to BeamEnd
	FHitResult WeaponHitRelust;
	const FVector WeaponStartPoint{ SocketBeam };
	const FVector StartToEnd{ OutBeamLocation - SocketBeam };
	const FVector WeaponEndPoint{ SocketBeam + StartToEnd  *1.25f};
	GetWorld()->LineTraceSingleByChannel(
		WeaponHitRelust,
		WeaponStartPoint,
		WeaponEndPoint,
		ECollisionChannel::ECC_Visibility);

	//Change BeamEnd if beetwen barrel and BeamEnd have somethink
	if (WeaponHitRelust.bBlockingHit)
	{
		OutBeamLocation = WeaponHitRelust.Location; 
		return true;
	}
	return false;
}

void AShooterCharacter::AimButtonPressed()
{
	bAiming = true;
}

void AShooterCharacter::AimButtomReleased()
{
	bAiming = false;
}

void AShooterCharacter::InterpZoomFOV(float DeltaTime)
{
	if (bAiming)
	{
		CurrentCameraFOV = FMath::FInterpTo(CurrentCameraFOV, AimCameraFOV, DeltaTime, InterpZoomSpeed);
	}
	else
	{
		CurrentCameraFOV = FMath::FInterpTo(CurrentCameraFOV, DefaultCameraFOV, DeltaTime, InterpZoomSpeed);
	}
	FollowCamera->SetFieldOfView(CurrentCameraFOV);
}
