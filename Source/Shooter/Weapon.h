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
public:
	void ThrowWeapon();


};
