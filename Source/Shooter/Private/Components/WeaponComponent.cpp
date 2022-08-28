// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"
#include "../ShooterCharacter.h"
#include "../Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent() :
	bShooting(false),
	CrosshiresDelayTimer(0.05f),
	//AutoShooting Valiables
	bFireButtonPressed(false),
	bShouldFire(true),
	ShootingRate(0.1f),
	//Start Ammo value
	Init9mmAmmo(9),
	InitARAmmo(30)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UWeaponComponent::ReloadWeapon()
{
	if (!CharOwner || CharOwner->GetECombatState() != ECombatState::ECS_Unoccupied) return;
	if (!EquippedWeapon || EquippedWeapon->IsMagazineFull()) return;

	if (CarryingAmmo())
	{
		const auto AnimInstance = CharOwner->GetMesh()->GetAnimInstance();
		if (ReloadMontage && AnimInstance)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(EquippedWeapon->GetMontageWeaponSectionName());
			CharOwner->SetECombatState(ECombatState::ECS_Reloading);
			bAiming = false;
		}
	}
}

void UWeaponComponent::FinishReloading()
{
	if (!CharOwner) return;
	CharOwner->SetECombatState(ECombatState::ECS_Unoccupied);

	if (!EquippedWeapon) return;
	const auto AmmoType = EquippedWeapon->GetAmmoType();
	if (!AmmoCharacter.Contains(AmmoType)) return;


	const int32 EmptySpaceMagazine = EquippedWeapon->GetMagazineCapacity() - EquippedWeapon->GetAmmo();
	if (EmptySpaceMagazine > AmmoCharacter[AmmoType])
	{
		//Take all Character ammo for magazine
		EquippedWeapon->ReloadMagazine(AmmoCharacter[AmmoType]);
		AmmoCharacter.Add(AmmoType, 0);
	}
	else
	{
		// Take only what we need bullet
		EquippedWeapon->ReloadMagazine(EmptySpaceMagazine);
		const int32 BulletSpend = AmmoCharacter[AmmoType] - EmptySpaceMagazine;
		AmmoCharacter.Add(AmmoType, BulletSpend);
	}
}

bool UWeaponComponent::CarryingAmmo()
{
	if (!EquippedWeapon) return false;
	const auto WeaponAmmoType = EquippedWeapon->GetAmmoType();
	if (AmmoCharacter.Contains(WeaponAmmoType))
	{
		return AmmoCharacter[WeaponAmmoType] > 0;
	}
	return false;
}

void UWeaponComponent::TakeClip()
{
	if (!CharOwner || EquippedWeapon || !CharOwner->GetHandSceneComponent()) return;
	const int32 BoneIndexClip = EquippedWeapon->GetSkeletalMeshComponent()->GetBoneIndex(EquippedWeapon->GetClipBoneName());
	const FTransform BoneTransformClip = EquippedWeapon->GetSkeletalMeshComponent()->GetBoneTransform(BoneIndexClip);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	CharOwner->GetHandSceneComponent()->AttachToComponent(CharOwner->GetMesh(), AttachmentRules, FName(TEXT("Hand_L")));
	CharOwner->GetHandSceneComponent()->SetWorldTransform(BoneTransformClip);

	EquippedWeapon->SetIsClipTaked(true);
}

void UWeaponComponent::ReturnClip()
{
	EquippedWeapon->SetIsClipTaked(false);
}

//spawn weapon when we start
AWeapon* UWeaponComponent::SpawnDefaultWeapon()
{
	if (DefaultWeapon) //if we have one
	{
		AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultWeapon);//spawn weapon what we chose
		return SpawnedWeapon;
	}
	return nullptr;
}

void UWeaponComponent::EquipWeapon(AWeapon* EquipWeapon)
{
	if (!EquipWeapon || !CharOwner) return;
	EquipWeapon->GetCollisionBox()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	EquipWeapon->GetAgroSphere()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (EquipWeapon)
	{
		const USkeletalMeshSocket* HandSocket = CharOwner->GetMesh()->GetSocketByName(FName("hand_rSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(EquipWeapon, CharOwner->GetMesh());//...get socket and attach weapon for him
		}
		EquippedWeapon = EquipWeapon; // set equippedWEapon
		EquippedWeapon->SetItemStates(EItemStates::EIS_Equipped);//Change Item State
	}
}

void UWeaponComponent::DropWeapon()
{
	if (EquippedWeapon)
	{
		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedWeapon->GetSkeletalMeshComponent()->DetachFromComponent(DetachmentTransformRules);

		EquippedWeapon->SetItemStates(EItemStates::EIS_Falling);
		EquippedWeapon->ThrowWeapon();
	}
}

void UWeaponComponent::SwapWeapon(AWeapon* SwapWeapon)
{
	DropWeapon();
	EquipWeapon(SwapWeapon);
}

void UWeaponComponent::InitAmmo()
{
	AmmoCharacter.Add(EAmmoType::EAT_9MM, Init9mmAmmo);
	AmmoCharacter.Add(EAmmoType::EAT_AR, InitARAmmo);

}

bool UWeaponComponent::IsAmmoEmpty()
{
	if (!EquippedWeapon) return true;
	return EquippedWeapon->GetAmmo() <= 0;
}

void UWeaponComponent::PlayFireSound()
{
	//play fire sound
	if (FireShotSound)
	{
		UGameplayStatics::PlaySound2D(this, FireShotSound);
	}
}

void UWeaponComponent::SendBullet()
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

void UWeaponComponent::PlayGunFireMontage()
{
	if (!CharOwner) return;
	//AnimMontage for HipFireAnimation 
	UAnimInstance* AnimInstance = CharOwner->GetMesh()->GetAnimInstance();
	if (AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("Start_Fire"));
	}
}

void UWeaponComponent::FireWeapon()
{
	if (!EquippedWeapon || !CharOwner) return;
	if (CharOwner->GetECombatState() != ECombatState::ECS_Unoccupied) return;

	if (!IsAmmoEmpty())
	{
		PlayFireSound();
		SendBullet();
		PlayGunFireMontage();
		//decrement bullet current weapon
		EquippedWeapon->DecrementAmmo();

		StartAutoFire();
	}
}

bool UWeaponComponent::GetBeamEndLocation(const FVector& SocketBeam, FVector& OutBeamLocation)
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
	const FVector WeaponEndPoint{ SocketBeam + StartToEnd * 1.25f };
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

void UWeaponComponent::StartCrosshiresFire()
{
	if (!GetWorld()) return;
	bShooting = true;
	GetWorld()->GetTimerManager().SetTimer(CrosshiresTimer, this, &UWeaponComponent::EndCrosshiresFire, CrosshiresDelayTimer);
}

void UWeaponComponent::EndCrosshiresFire()
{
	bShooting = false;
}

void UWeaponComponent::StartAutoFire()
{
	if (!CharOwner || !GetWorld())return;
	CharOwner->SetECombatState(ECombatState::ECS_FireTimerProgress);

	GetWorld()->GetTimerManager().SetTimer(ShootingTimer, this, &UWeaponComponent::ResetAutoShooting, ShootingRate);
}

void UWeaponComponent::ResetAutoShooting()
{
	if (!CharOwner) return;
	CharOwner->SetECombatState(ECombatState::ECS_Unoccupied);

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

bool UWeaponComponent::ToogleVisibilityWidgetPickUp(FHitResult& PickUpitem, FVector& OutVector)
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
		if (!GetWorld())return false;
		GetWorld()->LineTraceSingleByChannel(PickUpitem, StartPoint, EndPoint, ECollisionChannel::ECC_Visibility);

		if (PickUpitem.bBlockingHit)// was there a trace hit?
		{
			return true;
		}
	}
	return false;
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	CharOwner = Cast<AShooterCharacter>(GetOwner());

	InitAmmo();
	//spawn default weapon and Equip Weapon
	EquipWeapon(SpawnDefaultWeapon());
	// ...
}

void UWeaponComponent::ReloadButtonPressed()
{
	ReloadWeapon();
}

void UWeaponComponent::FireButtonPressed()
{
	bFireButtonPressed = true;
	FireWeapon();
}

void UWeaponComponent::FireButtonReleased()
{
	bFireButtonPressed = false;
}

void UWeaponComponent::AimButtonPressed()
{
	bAiming = true;
}

void UWeaponComponent::AimButtomReleased()
{
	bAiming = false;
}


//// Called every frame
//void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

