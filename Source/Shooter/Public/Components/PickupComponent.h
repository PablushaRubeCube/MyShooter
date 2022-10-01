// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PickupComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPickupComponent();

private://variables

	FTimerHandle PickupSoundTimer;
	bool bCanPlayPickupSound = true;

private://functions

	void ResumePickupSound();

protected://variables

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float SkipPickupSoundTime = 0.2f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void HoldPickupSound();

	FORCEINLINE bool GetbCanPlayPickupSound() const { return bCanPlayPickupSound; }
		
};
