// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"

void UEnemyAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	const auto Pawn = TryGetPawnOwner();
	if (!Pawn) return;
	Speed = Pawn->GetVelocity().Size();
}
