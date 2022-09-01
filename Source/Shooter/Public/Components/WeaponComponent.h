// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../AmmoType.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

private://variables

	UPROPERTY()
	class AShooterCharacter* CharOwner = nullptr;

	//Chose Weapon for Spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeapon> DefaultWeapon;

	//EquippedWeapon our Char
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class AWeapon* EquippedWeapon = nullptr;

	// store Reload montage 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	//HipValue to Lookup our char Gamepad
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bAiming = false;

	//Store Char Ammo
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoCharacter;

	//sound cue for gun shots
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireShotSound;

	//particle effect for gun fire
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	//particle effect in shotend
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	//beam smoke
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;

	//combat montage for shot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;

	//Crosshair 
	FTimerHandle CrosshiresTimer;
	bool bShooting = false;
	float CrosshiresDelayTimer = 0.05f;

	//Variables for AutoShooting
	bool bFireButtonPressed = false;
	bool bShouldFire = false;
	float ShootingRate = 0.1f;
	FTimerHandle ShootingTimer;

	//Start int 9mm ammo
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 Init9mmAmmo = 9;
	//Start int AR ammo
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	int32 InitARAmmo = 30;
	bool bIsWasAiming = false;

private://functions

	/** Main Function When we Reload*/
	void ReloadWeapon();

	//Call When reload animation near to end
	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	// Check if current wepon have ammo
	bool CarryingAmmo();

	// Attach clip current weapon when we reload
	UFUNCTION(BlueprintCallable)
	void TakeClip();

	// Released clip current weapon when we finish reload
	UFUNCTION(BlueprintCallable)
	void ReturnClip();

	//Spawn Weapon when we start
	class AWeapon* SpawnDefaultWeapon();

	//EquipWeapon
	void EquipWeapon(AWeapon* EquipWeapon);

	//Drop Weapon
	void DropWeapon();

	// init Start Char Ammo
	void InitAmmo();

	/**if ammo current weapon empty return true */
	bool IsAmmoEmpty();

	/** Play sound if we fire*/
	void PlayFireSound();

	/** main function when we fire*/
	void SendBullet();

	/** play anim when we fire*/
	void PlayGunFireMontage();

	//Open fire when we pressed fireButton
	void FireWeapon();

	//Create and return BeamLocation for particles shots 
	bool GetBeamEndLocation(const FVector& SocketBeam, FVector& OutBeamLocation);

	//Change Crosshire Spread when we shoting
	void StartCrosshiresFire();
	UFUNCTION()
	void EndCrosshiresFire();

	//Timer For AutoShooting
	void StartAutoFire();
	UFUNCTION()
	void ResetAutoShooting();

	bool IsCanAim();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Return Aiming condition our char
	bool GetAimingCondition();

	//Swap Weapon
	void SwapWeapon(AWeapon* SwapWeapon);

	/** Call when we press the button*/
	void ReloadButtonPressed();

	//Call when we pressed LMB
	void FireButtonPressed();
	void FireButtonReleased();

	//change camera when we aim 
	void AimButtonPressed();
	void AimButtomReleased();

	bool GetbShooting() const { return bShooting; }

	void AddAmmo(class AAmmo* Ammo);
};
