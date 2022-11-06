// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "Enemy/AIComponent.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Components/HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HitNumberWidget.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogEnemyCharacter, All, All)

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AIComponent = CreateDefaultSubobject<UAIComponent>(TEXT("AIComponent"));

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(AIComponent);
	
}



// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHitNumberWidgetLocations();
}

// Called to bind functionality to input
//void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

void AEnemyCharacter::BulletHit_Implementation(const FHitResult& HitResult)
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, HitResult.Location);
	UGameplayStatics::SpawnEmitterAtLocation
	(this, ImpactParticle, HitResult.Location,FRotator::ZeroRotator, true);
	AIComponent->SetIsStunned(true);
	PlayAnimMontage(GetHitMontage());
}

void AEnemyCharacter::DisplayDamageNumber(const int32 Damage, const FVector Location,  APlayerController* const PlayerController, const bool bIsHeadShoot)
{
	CharController = PlayerController;
	const auto NumberWidget = CreateWidget<UHitNumberWidget>(GetWorld(), HitNumberWidget);
	if (!NumberWidget)return;
	NumberWidget->AddToViewport();
	SetPositionOnTheScreen(NumberWidget, PlayerController, Location);
	NumberWidget->SetDamage(Damage);
	NumberWidget->PlayFadeAnimation();
	NumberWidget->SetHeadShoot(bIsHeadShoot);
	HitNumberWidgets.Add(NumberWidget, Location);
	StartTimerToDestroyWidget(NumberWidget);
}



void AEnemyCharacter::SetPositionOnTheScreen(UUserWidget* const& NumberWidget, APlayerController* const& PlayerController, const FVector& Location)
{
	FVector2D ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(PlayerController, Location, ScreenPosition);
	NumberWidget->SetPositionInViewport(ScreenPosition);
}

void AEnemyCharacter::StartTimerToDestroyWidget(UUserWidget* Widget)
{
	FTimerHandle TimerDamageWidget;
	FTimerDelegate HitNumberDelegate;
	HitNumberDelegate.BindUFunction(this, FName("DestroyWidget"), Widget);
	GetWorldTimerManager().SetTimer(TimerDamageWidget, HitNumberDelegate, TimeToHideWidget,false);
}

void AEnemyCharacter::DestroyWidget(UUserWidget* Widget)
{
	HitNumberWidgets.Remove(Widget);
	Widget->RemoveFromParent();
}

void AEnemyCharacter::UpdateHitNumberWidgetLocations()
{
	if (!CharController) return;
	for(auto& Widget : HitNumberWidgets)
	{
	SetPositionOnTheScreen(Widget.Key, CharController, Widget.Value);
	}
}

bool AEnemyCharacter::GetRandomAttackSectionName(FName& SectionName)
{
	const auto index = FMath::RandRange(0, AttackSectionName.Num()-1);
	if (AttackSectionName.IsValidIndex(index))
	{
		SectionName = AttackSectionName[index];
		return true;
	}
	return false;
}



