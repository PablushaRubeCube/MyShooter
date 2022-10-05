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

private://Functions

	void SetWeaponProperties();

protected://variables

	//Store anim montage Name current weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MontageWeaponName = "Reload_SMG";

	// Store Clip weapon bone Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName ClipBoneName = "smg_clip";

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool IsClipTaked = false;

	int32 SlotNumber;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UDataTable* WeaponPropertiesData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	FWeaponPropertiesTable WeaponPropertiesRow;

protected://functions

	void StopFalling();

	virtual void GetPickupItem() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	/** Call when player Throw Weapon */
	void ThrowWeapon();

	/**Get Ammo Current Weapon  */
	FORCEINLINE int32 GetAmmo() const { return WeaponPropertiesRow.Ammo; }
	/**Get Max Current Weapon Magazine Capacity  */
	FORCEINLINE int32 GetMagazineCapacity() const { return WeaponPropertiesRow.MagazineCapacity; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponPropertiesRow.WeaponType; }
	UFUNCTION(BlueprintPure)
	EAmmoType GetAmmoType() const { return WeaponPropertiesRow.AmmoType; }

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
