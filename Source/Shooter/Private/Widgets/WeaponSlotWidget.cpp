// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WeaponSlotWidget.h"
#include "../ShooterCharacter.h"
#include "Components/InventoryComponent.h"
#include "../Weapon.h"
#include "stdafx.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponSlotWidget, All, All)

EItemRare UWeaponSlotWidget::GetWeaponRare()
{
	const auto Weapon = GetWeapon();
	return Weapon ? Weapon->GetItemRare() : EItemRare::EIR_MAX;
}

EWeaponType UWeaponSlotWidget::GetWeaponType()
{
	const auto Weapon = GetWeapon();
	return Weapon ? Weapon->GetWeaponType() : EWeaponType::EWT_MAX;
}

EAmmoType UWeaponSlotWidget::GetWeaponAmmoType()
{
	const auto Weapon = GetWeapon();
	return Weapon ? Weapon->GetAmmoType() : EAmmoType::EAT_MAX;
}

bool UWeaponSlotWidget::GetWeaponAmmo(int32& Ammo)
{
	const auto Weapon = GetWeapon();
	if (Weapon)
	{
		Ammo = Weapon->GetAmmo();
		return true;
	}
	return false;
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

void UWeaponSlotWidget::PlayChooseAnimation()
{
	PlayAnimation(GetSelectAnimation());
	UE_LOG(LogWeaponSlotWidget, Display, TEXT("PlayChoose %s"), *this->GetName());
}

void UWeaponSlotWidget::PlayReverseChooseAnimation()
{
	PlayAnimationReverse(GetSelectAnimation());
	UE_LOG(LogWeaponSlotWidget, Display, TEXT("PlayReverse %s"), *this->GetName());
}