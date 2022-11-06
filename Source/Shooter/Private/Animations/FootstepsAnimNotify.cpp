// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/FootstepsAnimNotify.h"
#include "ShooterCharacter.h"
#include "ShooterCoreTypes.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void UFootstepsAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	check(DefaultMaterialToFootsteps);
	const auto Char = Cast<AShooterCharacter>(MeshComp->GetOwner());
	if (!Char)return;
	UPhysicalMaterial* PhysicalMaterial = Char->GetFootstepPhysicalMaterial();
	if (PhysicalMaterial)
	{
		if (!FootstepsFX.Contains(PhysicalMaterial)) PhysicalMaterial = DefaultMaterialToFootsteps;
		UGameplayStatics::PlaySoundAtLocation(Char, FootstepsFX[PhysicalMaterial].FootStepsSound, MeshComp->GetBoneLocation(BoneName));
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(Char, FootstepsFX[PhysicalMaterial].FootstepsParticles, MeshComp->GetBoneLocation(BoneName));
	}
}
