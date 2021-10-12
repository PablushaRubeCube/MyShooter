// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

AWeapon::AWeapon():
TimeThrowWeapon (0.75),
bWeaponFalling (false),
MeshRotation(0.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetItemStates() == (EItemStates::EIS_Falling) && bWeaponFalling)
	{
		//Rotate Mesh onlyYaw when we Throwed Weapon
		FRotator RotationMesh(0.f, GetSkeletalMeshComponent()->GetComponentRotation().Yaw, 0.f);
		GetSkeletalMeshComponent()->SetWorldRotation(RotationMesh, false, nullptr, ETeleportType::TeleportPhysics);

	}

}

void AWeapon::ThrowWeapon()
{
	//
	bWeaponFalling = true;
	//Rotate Mesh onlyYaw when we Throwed Weapon
	FRotator RotationMesh{ 0.f, GetSkeletalMeshComponent()->GetComponentRotation().Yaw, 0.f };
	GetSkeletalMeshComponent()->SetWorldRotation(RotationMesh, false, nullptr, ETeleportType::TeleportPhysics);

	//GetVEctor
	const FVector ForwardVector { GetSkeletalMeshComponent()->GetForwardVector() };
	const FVector RightVector { GetSkeletalMeshComponent()->GetRightVector() };

	//Caclulate Direction Impulse
	FVector ImpulseDirection = RightVector.RotateAngleAxis(-20.f, ForwardVector);
	float RandomRotation = { 30.f };
	ImpulseDirection = RightVector.RotateAngleAxis(RandomRotation, FVector(0.f,0.f,1.f));
	ImpulseDirection *= 4000.f;

	//Throw
	GetSkeletalMeshComponent()->AddImpulse(ImpulseDirection);

	//When Time is over change State and "bWeaponFalling"
	GetWorldTimerManager().SetTimer(TimerThrowWeapon,this, &AWeapon::StopFalling, TimeThrowWeapon, false);
}

void AWeapon::StopFalling()
{

	bWeaponFalling = false;
	SetItemStates(EItemStates::EIS_Pickup);
}