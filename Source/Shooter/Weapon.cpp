// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "ShooterCharacter.h"
#include "Components/WeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All)

AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool AWeapon::IsMagazineFull()
{
	return MagazineCapacity == Ammo;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetItemStates() == (EItemStates::EIS_Falling) && bWeaponFalling)
	{
		//Rotate Mesh onlyYaw when we Throwed Weapon
		FRotator RotationMesh(0.f, GetSkeletalMeshComponent()->GetComponentRotation().Yaw, 0.f);
		GetSkeletalMeshComponent()->SetWorldRotation(RotationMesh, false, nullptr, ETeleportType::TeleportPhysics);

	}

}

void AWeapon::ThrowWeapon()
{
	//
	bWeaponFalling = true;
	//Rotate Mesh onlyYaw when we Throwed Weapon
	FRotator RotationMesh{ 0.f, GetSkeletalMeshComponent()->GetComponentRotation().Yaw, 0.f };
	GetSkeletalMeshComponent()->SetWorldRotation(RotationMesh, false, nullptr, ETeleportType::TeleportPhysics);

	//GetVEctor
	const FVector ForwardVector { GetSkeletalMeshComponent()->GetForwardVector() };
	const FVector RightVector { GetSkeletalMeshComponent()->GetRightVector() };
	//Caclulate Direction Impulse
	FVector ImpulseDirection = RightVector.RotateAngleAxis(-20.f, ForwardVector);
	float RandomRotation = { 30.f };
	ImpulseDirection = RightVector.RotateAngleAxis(RandomRotation, FVector(0.f,0.f,1.f));
	ImpulseDirection *= 4000.f;

	//Throw
	GetSkeletalMeshComponent()->AddImpulse(ImpulseDirection);

	//When Time is over change State and "bWeaponFalling"
	GetWorldTimerManager().SetTimer(TimerThrowWeapon,this, &AWeapon::StopFalling, TimeThrowWeapon, false);
}

void AWeapon::DecrementAmmo()
{
	Ammo <= 0 ? Ammo = 0 : Ammo--;
}

void AWeapon::ReloadMagazine(int32 Value)
{
	checkf(MagazineCapacity >= Ammo + Value, TEXT("Reload Ammo more than MagazineCapacity"));

	Ammo += Value;
	//if (MagazineCapacity >= Ammo + Value)
	//{
	//	
	//}
}

void AWeapon::StopFalling()
{

	bWeaponFalling = false;
	SetItemStates(EItemStates::EIS_Pickup);
}

void AWeapon::GetPickupItem()
{
	const auto Char = Cast<AShooterCharacter>(GetOwner());
	if (!Char) return;
	UGameplayStatics::PlaySound2D(this, GetEquipSound());
	const auto WeaponComponent = Cast<UWeaponComponent>(Char->GetComponentByClass(UWeaponComponent::StaticClass()));
	if (!WeaponComponent) return;
	WeaponComponent->SwapWeapon(this);
}
