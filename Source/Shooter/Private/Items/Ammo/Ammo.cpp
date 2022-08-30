// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Ammo/Ammo.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

AAmmo::AAmmo()
{
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	SetRootComponent(AmmoMesh);

	CollisionBox->SetupAttachment(GetRootComponent());
	AgroSphere->SetupAttachment(GetRootComponent());
	PickUpWidget->SetupAttachment(GetRootComponent());
}

//void AAmmo::SetItemProperties(EItemStates State, UMeshComponent* Mesh)
//{
//	Super::SetItemProperties(State, AmmoMesh);
//}
