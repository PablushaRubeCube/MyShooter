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

	void SetItemProperties();

protected://variables

	//Skeletal Mesh of item
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* ItemMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool IsClipTaked = false;

	int32 SlotNumber;

	/**Store type of Weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	UDataTable* ItemPropertiesToWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UDataTable* WeaponPropertiesData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FWeaponPropertiesTable WeaponPropertiesRow;

	TMap<EWeaponType, FName> RowName{
			{EWeaponType::EWT_SubmachineGun, TEXT("SubmachineGun")},
			{EWeaponType::EWT_AssaultRifle, TEXT("AssaultRifle")},
			{EWeaponType::EWT_Pistol, TEXT("Pistol")} };

protected://functions

	void StopFalling();

	virtual void GetPickupItem() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual UMeshComponent* GetMeshComponent() override { return ItemMesh; }
	
public:
	/** Call when player Throw Weapon */
	void ThrowWeapon();

	/**Get Ammo Current Weapon  */
	FORCEINLINE int32 GetAmmo() const { return WeaponPropertiesRow.Ammo; }
	/**Get Max Current Weapon Magazine Capacity  */
	FORCEINLINE int32 GetMagazineCapacity() const { return WeaponPropertiesRow.MagazineCapacity; }
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	UFUNCTION(BlueprintPure)
	EAmmoType GetAmmoType() const { return WeaponPropertiesRow.AmmoType; }

	FORCEINLINE FName GetMontageWeaponSectionName() const { return WeaponPropertiesRow.MontageWeaponName; }
	FORCEINLINE FName GetClipBoneName() const { return WeaponPropertiesRow.ClipBoneName; }
	FORCEINLINE void SetIsClipTaked(bool ClipState) { IsClipTaked = ClipState; }

	/** Decrement Ammo when we Shoot */
	void DecrementAmmo();
	/** Reload Magazine Current Weapon*/
	void ReloadMagazine(int32 Value);
	bool IsMagazineFull();

	FORCEINLINE USkeletalMeshComponent* GetSkeletalMeshComponent() const { return ItemMesh; }
};
