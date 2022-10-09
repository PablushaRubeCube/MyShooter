// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../AmmoType.h"
#include "Ammo.generated.h"

UCLASS()
class SHOOTER_API AAmmo : public AItem
{
	GENERATED_BODY()

public:

	AAmmo();

protected://variables

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USphereComponent* PickupSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* AmmoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	EAmmoType AmmoType = EAmmoType::EAT_MAX;

	int32 IndexInterpLocation = 0;

	//Amount our ammo
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 AmmoAmount = 0.f;

	/** Sound pickup current item */
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundCue* PickupSound;

	/** Sound Equip current item */
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundCue* EquipSound;

	/** Set Pickup widget Icon*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	UTexture2D* AmmoIcon;

protected://functions
	
	virtual UMeshComponent* GetMeshComponent()  override { return AmmoMesh; }

	virtual void GetPickupItem() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public://functions

	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

	FORCEINLINE int32 GetIndexInterpLocation() const { return IndexInterpLocation; }

	FORCEINLINE int32 GetAmmoAmount() const { return AmmoAmount; }

	//Char call when start pickup item
	virtual	void StartPickupItem(AShooterCharacter* Char) override;

	virtual class USoundCue* GetPickupSound() override { return PickupSound; }
	virtual USoundCue* GetEquipSound() override { return EquipSound; }
};
