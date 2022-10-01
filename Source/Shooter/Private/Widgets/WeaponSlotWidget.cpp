// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WeaponSlotWidget.h"
#include "../ShooterCharacter.h"
#include "Components/InventoryComponent.h"
#include "../Weapon.h"

EItemRare UWeaponSlotWidget::GetWeaponRare()
{
	const auto Weapon = GetWeapon();
	return Weapon ? Weapon->GetItemRare() : EItemRare::EIR_Max;
}

EWeaponType UWeaponSlotWidget::GetWeaponType()
{
	const auto Weapon = GetWeapon();
	return Weapon ? Weapon->GetWeaponType() : EWeaponType::EWT_MAX;
}

AWeapon* UWeaponSlotWidget::GetWeapon()
{
	const auto Char = Cast<AShooterCharacter>(GetOwningPlayerPawn());
	if (!Char) return nullptr;
	const auto InventoryComponent = Cast<UInventoryComponent>(Char->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (!InventoryComponent) return nullptr;
	AWeapon* Weapon = InventoryComponent->GetSpecificWeapon(NumberWeaponSlot);
	return Weapon;
}
