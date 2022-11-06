// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private://variables

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Speed;

private://functions

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);
};