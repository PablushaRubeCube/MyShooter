// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item.h"
#include "../AmmoType.h"
#include "Ammo.generated.h"

/**
 * 
 */
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

protected://functions
	
	virtual UMeshComponent* GetMeshComponent() const override { return AmmoMesh; }

	virtual void GetPickupItem() override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



public://functions

	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

	FORCEINLINE int32 GetIndexInterpLocation() const { return IndexInterpLocation; }

	//Char call when start pickup item
	virtual	void StartCurveItem(AShooterCharacter* Char) override;
};
