// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
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
protected:

	void StopFalling();
private:

	FTimerHandle TimerThrowWeapon;
	float TimeThrowWeapon;
	bool bWeaponFalling;
	FRotator MeshRotation;

	/**Store Ammo Value current Weapon*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 Ammo;

public:
	/** Call when player Throw Weapon */
	void ThrowWeapon();

	/**Get Ammo Current Weapon  */
	FORCEINLINE int32 GetAmmo() const { return Ammo; }

	/** Decrement Ammo when we Shoot */
	void DecrementAmmo();

};
