// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive/Explosive.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AExplosive::AExplosive()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void AExplosive::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosive::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosive::BulletHit_Implementation(const FHitResult& HitResult)
{
	UGameplayStatics::PlaySoundAtLocation(this, ExplosiveSound, HitResult.Location);
	UGameplayStatics::SpawnEmitterAtLocation
	(this, ExplosiveParticle, HitResult.Location, FRotator::ZeroRotator, true);
	Destroy();
}

