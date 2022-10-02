// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Components/WeaponComponent.h"
#include "Widgets/InventoryWidget.h"
#include "ShooterPlayerController.h"
#include "ShooterCharacter.h"
#include "Weapon.h"

DEFINE_LOG_CATEGORY_STATIC(LogInventoryComponent, All, All)

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddWeaponToList(AWeapon* Weapon)
{
	if (!Weapon) return;
	if (WeaponList.Num() < MaxWeaponList)
	{
		WeaponList.Add(Weapon);
	}
	else
	{
		bIsInventoryFull = true;
		WeaponList[PreviousSelectWeaponIndex] = Weapon;
	}
}

AWeapon* UInventoryComponent::GetSpecificWeapon(int32 WeaponNumber) const
{
	if (WeaponNumber >= WeaponList.Num())
	{
		return nullptr;
	}
	return WeaponList[WeaponNumber];
}

void UInventoryComponent::ChooseInventoryItem(int32 IndexItem)
{
	AShooterCharacter* Char = Cast<AShooterCharacter>(GetOwner());
	if (!Char || !WeaponList.IsValidIndex(IndexItem)) return;
	if (Char->GetECombatState() == ECombatState::ECS_Unoccupied || Char->GetECombatState() == ECombatState::ECS_Equiping)
	{
		const auto WeaponComponent = Cast<UWeaponComponent>(GetOwner()->GetComponentByClass(UWeaponComponent::StaticClass()));
		if (!WeaponComponent) return;
		if (WeaponComponent->GetEquippedWeapon())
		{
			WeaponComponent->GetEquippedWeapon()->SetItemStates(EItemStates::EIS_PickedUp);
		}
		OnChooseItem.Broadcast(IndexItem, PreviousSelectWeaponIndex);
		PreviousSelectWeaponIndex = IndexItem;
		if (WeaponList.IsValidIndex(IndexItem))
		{
			const auto Weapon = WeaponList[IndexItem];
			if (Weapon)
				WeaponComponent->EquipWeapon(Weapon);
			Char->PlayAnimMontage(EquipMontage);
		}
	}
}

bool UInventoryComponent::GetNextEmptyInventorySlot(int32& WeaponSlotIndex) const
{
	if (!IsInvetoryFull())
	{
		WeaponSlotIndex = WeaponList.Num();
		return true;
	}
	return false;
}


