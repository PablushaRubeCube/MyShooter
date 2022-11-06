// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Ammo/Ammo.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "../ShooterCharacter.h"
#include "Components/WeaponComponent.h"

AAmmo::AAmmo()
{
	AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoMesh"));
	SetRootComponent(AmmoMesh);

	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpSphere"));
	PickupSphere->SetupAttachment(GetRootComponent());

	CollisionBox->SetupAttachment(GetRootComponent());
	AgroSphere->SetupAttachment(GetRootComponent());
	PickUpWidget->SetupAttachment(GetRootComponent());
}

void AAmmo::GetPickupItem()
{
	const auto Char = Cast<AShooterCharacter>(GetOwner());
	const auto WeaponComponent = Cast<UWeaponComponent>(Char->GetComponentByClass(UWeaponComponent::StaticClass()));
	if (!WeaponComponent)return;
	WeaponComponent->AddAmmo(this);
	Destroy();
}

void AAmmo::BeginPlay()
{
	Super::BeginPlay();

	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::OnPickupSphereBeginOverlap);
}

void AAmmo::OnPickupSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto Char = Cast<AShooterCharacter>(OtherActor);
	if (!Char) return;
	StartPickupItem(Char);
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAmmo::StartPickupItem(AShooterCharacter* Char)
{
	Super::StartPickupItem(Char);

	IndexInterpLocation = Char->GetInterpIndexLocation();
}