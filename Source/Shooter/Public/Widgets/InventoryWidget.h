// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

private://variables
	//Need change nullptr to current WBP_WeaponSlotWidets in BP
	UPROPERTY(EditAnywhere,BlueprintReadWrite, category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<FName, class UWeaponSlotWidget*> ItemWidets;

	UPROPERTY(BlueprintReadOnly, category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UWeaponSlotWidget* SelectedWidget;

	UPROPERTY(BlueprintReadOnly, category = "Inventory", meta = (AllowPrivateAccess = "true"))
	UWeaponSlotWidget* PreviousSelectedWidget;

private://functions
	void ChooseItem(FName ButtonName);

protected://functinos
	virtual void NativeOnInitialized() override;

public://functions

};
