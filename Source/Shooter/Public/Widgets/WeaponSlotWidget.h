// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Item.h"
#include "../Weapon.h"
#include "WeaponSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UWeaponSlotWidget : public UUserWidget
{
	GENERATED_BODY()

private://variables

	//Store Self slot Number for weapon choose
	UPROPERTY(EditAnywhere, category = "Weapon", meta = (AllowPrivateAccess = true))
	int32 NumberWeaponSlot = INT32_MAX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Weapon", meta = (AllowPrivateAccess = true))
	TMap<EItemRare, UTexture2D*> RareBackgorundTextures;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Weapon", meta = (AllowPrivateAccess = true))
	TMap<EWeaponType, UTexture2D*> WeaponTextures;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Weapon", meta = (AllowPrivateAccess = true))
	TMap<EAmmoType, UTexture2D*> AmmoTextures;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* SelectAnimation;

private://functions

	UFUNCTION(BlueprintPure)
	EItemRare GetWeaponRare();

	UFUNCTION(BlueprintPure)
	EWeaponType GetWeaponType();

	UFUNCTION(BlueprintPure)
	EAmmoType GetWeaponAmmoType();

	UFUNCTION(BlueprintPure)
	bool GetWeaponAmmo(int32& Ammo);
	
	class AWeapon* GetWeapon();

	UFUNCTION(BlueprintPure)
	UWidgetAnimation* GetSelectAnimation() { return SelectAnimation; }

public:// Functions

	void PlayChooseAnimation();
	void PlayReverseChooseAnimation();
};
