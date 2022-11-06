// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitNumberWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UHitNumberWidget : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 WidgetDamage;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsHeadShoot;
	
	UPROPERTY(meta = (BindWidgetAnim),Transient)
	UWidgetAnimation* FadeAnimation;

public:

	void SetDamage(const int32& Damage);

	void PlayFadeAnimation();

	void SetHeadShoot(bool bIsHeadshoot);
};
