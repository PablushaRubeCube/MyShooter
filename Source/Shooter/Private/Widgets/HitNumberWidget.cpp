// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HitNumberWidget.h"

void UHitNumberWidget::SetDamage(const int32& Damage)
{
	WidgetDamage = Damage;
}

void UHitNumberWidget::PlayFadeAnimation()
{
	PlayAnimation(FadeAnimation);
}

void UHitNumberWidget::SetHeadShoot(bool bIsHeadshoot)
{
	bIsHeadShoot = bIsHeadshoot;
}