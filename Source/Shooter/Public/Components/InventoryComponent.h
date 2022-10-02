// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChooseItem, int32 SelectWeapon, int32 PreviousSelectWeapon)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

private://variables

	UPROPERTY(VisibleAnywhere,Category = "Weapon", meta = (AllowPrivateAccess = true))
	TArray<class AWeapon*> WeaponList;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	int32 MaxWeaponList = 6;

	bool bIsInventoryFull = false;

	//For Widget inventory index//Change to 0 when game is start and spawn default weapon
	int32 PreviousSelectWeaponIndex = INT32_MAX;

protected://variables

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* EquipMontage;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public://variables

	FOnChooseItem OnChooseItem;

public:	//functions
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddWeaponToList(AWeapon* Weapon);

	AWeapon* GetSpecificWeapon(int32 WeaponNumber) const;

	bool IsInvetoryFull() const {return bIsInventoryFull;}

	void ChooseInventoryItem(int32 IndexItem);

	bool GetNextEmptyInventorySlot(int32& WeaponSlotIndex) const;
		
};
