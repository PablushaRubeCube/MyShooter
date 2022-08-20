// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AmmoType.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 
{
	EWT_SubmachineGun UMETA(DisplayName"SMG"),
	EWT_AssaultRifle UMETA(DisplayName "Rifle"),

	EWT_MAX UMETA(DisplayName "Default")
};
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
	float TimeThrowWeapon;
	bool bWeaponFalling;
	FRotator MeshRotation;

protected://variables

	/**Store Ammo Value current Weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 Ammo;

	/** Store Max Magazine Capacity*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	int32 MagazineCapacity;

	//Store type of Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType;

	//Store Type of use Ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EAmmoType AmmoType;

	//Store anim montage Name current weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName MontageWeaponName;

	// Store Clip weapon bone Name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName ClipBoneName;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool IsClipTaked;

protected://functions

	void StopFalling();

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

};
