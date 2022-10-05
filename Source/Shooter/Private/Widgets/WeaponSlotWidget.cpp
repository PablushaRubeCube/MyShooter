// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WeaponSlotWidget.h"
#include "../ShooterCharacter.h"
#include "Components/InventoryComponent.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "../Weapon.h"


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

void UWeaponSlotWidget::PlayPickupAnimation(int32 SlotIndex, bool bIsTraceForItem)
{
	if (NumberWeaponSlot == SlotIndex && bIsTraceForItem && !IsAnimationPlaying(PickupAnimation))
	{
		ArrowIcon->SetVisibility(ESlateVisibility::Visible);
		PickupBorder->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(PickupAnimation,0.f,0);
	}
	else if (NumberWeaponSlot != SlotIndex || (!bIsTraceForItem && IsAnimationPlaying(PickupAnimation)))
	{
		ArrowIcon->SetVisibility(ESlateVisibility::Hidden);
		PickupBorder->SetVisibility(ESlateVisibility::Hidden);
		StopAnimation(PickupAnimation);
	}
}

void UWeaponSlotWidget::PlayChooseAnimation()
{
	PlayAnimation(GetSelectAnimation());
}

void UWeaponSlotWidget::PlayReverseChooseAnimation()
{
	PlayAnimationReverse(GetSelectAnimation());
}

void UWeaponSlotWidget::CheckItem(int32 SelectIndex, int32 PreviousSelectIndex)
{
	if (SelectIndex == PreviousSelectIndex || !GetWeapon()) return;
	if (SelectIndex == NumberWeaponSlot)
	{
		PlayChooseAnimation();
	}
	if (PreviousSelectIndex == NumberWeaponSlot)
	{
		PlayReverseChooseAnimation();
	}
}

void UWeaponSlotWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const auto Char = Cast<AShooterCharacter>(GetOwningPlayerPawn());
	if (!Char) return;
	Char->OnPickupEmptySlot.AddUObject(this, &UWeaponSlotWidget::PlayPickupAnimation);
	const auto Component = Cast<UInventoryComponent>(Char->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (!Component) return;
	Component->OnChooseItem.AddUObject(this, &UWeaponSlotWidget::CheckItem);

}