// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChooseItem, FName)

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

	void ChooseInventoryItem(FName PressedName);
		
};
