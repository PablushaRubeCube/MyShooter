// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryWidget.h"
#include "Widgets/WeaponSlotWidget.h"
#include "ShooterCharacter.h"
#include "Components/InventoryComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogInventoryWidget, All, All)

void UInventoryWidget::ChooseItem(FName ButtonName)
{
	if (PreviousSelectedWidget)PreviousSelectedWidget->PlayReverseChooseAnimation();
	if(ItemWidets.Contains(ButtonName))	SelectedWidget = (*ItemWidets.Find(ButtonName));
	if (SelectedWidget)SelectedWidget->PlayChooseAnimation();
	PreviousSelectedWidget = SelectedWidget;
}
void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	const auto Char = Cast<AShooterCharacter>(GetOwningPlayerPawn());
	if (!Char) return;
	const auto Component = Cast<UInventoryComponent>(Char->GetComponentByClass(UInventoryComponent::StaticClass()));
	if (!Component) return;
	Component->OnChooseItem.AddUObject(this, &UInventoryWidget::ChooseItem);
}

