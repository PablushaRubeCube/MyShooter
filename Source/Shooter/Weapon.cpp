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

	ItemMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);

	ItemRare = EItemRare::EIR_Damaged;

}

bool AWeapon::IsMagazineFull()
{
	return GetMagazineCapacity() == GetAmmo();
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
	GetAmmo() <= 0 ? WeaponPropertiesRow.Ammo = 0 : WeaponPropertiesRow.Ammo--;
}

void AWeapon::ReloadMagazine(int32 Value)
{
	checkf(GetMagazineCapacity() >= GetAmmo() + Value, TEXT("Reload Ammo more than MagazineCapacity"));

	WeaponPropertiesRow.Ammo += Value;
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
	WeaponComponent->PickupWeapon(this);
}

void AWeapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetWeaponProperties();
}

void AWeapon::SetWeaponProperties()
{
	if (WeaponPropertiesData && !RowName.Contains(WeaponType)) return;
	WeaponPropertiesRow = *WeaponPropertiesData->FindRow<FWeaponPropertiesTable>(*RowName.Find(WeaponType), TEXT(""));
	GetSkeletalMeshComponent()->SetSkeletalMesh(WeaponPropertiesRow.WeaponMesh);
	/*	switch (GetWeaponType())
		{
		case EWeaponType::EWT_SubmachineGun:
			WeaponPropertiesRow = *WeaponPropertiesData->FindRow<FWeaponPropertiesTable>(TEXT("SubmachineGun"),"");
			break;
		case EWeaponType::EWT_AssaultRifle:
			WeaponPropertiesRow = *WeaponPropertiesData->FindRow<FWeaponPropertiesTable>(TEXT("AssaultRifle"), "");
			break;
		case EWeaponType::EWT_Pistol:
			WeaponPropertiesRow = *WeaponPropertiesData->FindRow<FWeaponPropertiesTable>(TEXT("Pistol"), "");
			break;
		case EWeaponType::EWT_MAX:
			break;
		default:
			break;
		}*/
}