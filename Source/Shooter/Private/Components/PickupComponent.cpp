// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PickupComponent.h"

// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UPickupComponent::ResumePickupSound()
{
	bCanPlayPickupSound = true;
}

// Called when the game starts
void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPickupComponent::HoldPickupSound()
{
	bCanPlayPickupSound = false;
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().SetTimer(
		PickupSoundTimer,
		this,
		&UPickupComponent::ResumePickupSound,
		SkipPickupSoundTime);
}


// Called every frame
//void UPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

