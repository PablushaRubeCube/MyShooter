// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ShooterCoreTypes.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AWeapon();

private://variables

	FTimerHandle TimerThrowWeapon;
	float TimeThrowWeapon = 0.75f;
	bool bWeaponFalling = false;
	FRotator MeshRotation = FRotator::ZeroRotator;

protected://variables

	/**Store Ammo Value current Weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 Ammo = 30.f;

	/** Store Max Magazine Capacity*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 MagazineCapacity = 30.f;

	//Store type of Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::EWT_SubmachineGun;

	//Store Type of use Ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EAmmoType AmmoType = EAmmoType::EAT_9MM;

	//Store anim montage Name current weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MontageWeaponName = "Reload_SMG";

	// Store Clip weapon bone Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName ClipBoneName = "smg_clip";

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool IsClipTaked = false;

	int32 SlotNumber;

protected://functions

	void StopFalling();

	virtual void GetPickupItem() override;

public:
	/** Call when player Throw Weapon */
	void ThrowWeapon();

	/**Get Ammo Current Weapon  */
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	/**Get Max Current Weapon Magazine Capacity  */
	FORCEINLINE int32 GetMagazineCapacity() const { return MagazineCapacity; }

	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE FName GetMontageWeaponSectionName() const { return MontageWeaponName; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	FORCEINLINE void SetIsClipTaked(bool ClipState) { IsClipTaked = ClipState; }
	/** Decrement Ammo when we Shoot */
	void DecrementAmmo();

	/** Reload Magazine Current Weapon*/
	void ReloadMagazine(int32 Value);

	bool IsMagazineFull();

	virtual UMeshComponent* GetMeshComponent() const { return ItemMesh; }

};
