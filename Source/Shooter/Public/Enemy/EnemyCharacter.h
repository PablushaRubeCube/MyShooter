// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/BulletHitInterface.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class SHOOTER_API AEnemyCharacter : public ACharacter, public IBulletHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

private:

	UPROPERTY()
	APlayerController* CharController;

private://functions

	void SetPositionOnTheScreen(UUserWidget* const& NumberWidget, APlayerController* const& PlayerController, const FVector& Location);

protected://variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	class UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USphereComponent* AgroSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USphereComponent* AttackSphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	class UAIComponent* AIComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	TSubclassOf<UUserWidget> HitNumberWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget")
	float TimeToHideWidget {1.f};

	TMap<UUserWidget*, FVector> HitNumberWidgets;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TArray<FName> AttackSectionName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartTimerToDestroyWidget(UUserWidget* Widget);

	UFUNCTION()
	void DestroyWidget(UUserWidget* Widget);

	void UpdateHitNumberWidgetLocations();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(const FHitResult& HitResult) override;

	void DisplayDamageNumber(const int32 Damage, const FVector Location, APlayerController* const PlayerController, const bool bIsHeadShoot);

	FORCEINLINE USphereComponent* GetAgroSphere() const { return AgroSphere; }
	FORCEINLINE USphereComponent* GetAttackSphere() const { return AttackSphere; }

	UFUNCTION(BlueprintPure)
	UAnimMontage* GetAttackMontage() const { return AttackMontage; }

	UFUNCTION(BlueprintPure)
	bool GetRandomAttackSectionName(FName& SectionName);

	FORCEINLINE UAnimMontage* GetHitMontage() const { return HitMontage; }

};
