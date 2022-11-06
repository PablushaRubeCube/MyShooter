// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ShooterCoreTypes.h"
#include "FootstepsAnimNotify.generated.h"
/**
 * 
 */
UCLASS()
class SHOOTER_API UFootstepsAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected://variables

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "SkeletalMesh")
	FName BoneName;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UPhysicalMaterial* DefaultMaterialToFootsteps;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TMap< UPhysicalMaterial*,FFootstepsFX> FootstepsFX;
	
protected:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:

	

};
