// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIComponent.generated.h"

USTRUCT(BlueprintType)
struct FPointLocation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AI", meta = (MakeEditWidget))
	TArray<FVector> PointLocation;

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<FName> LocationName;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIComponent();

private:

	UPROPERTY()
	class AEnemyAIController* OwnController;

	UPROPERTY()
	class AEnemyCharacter* OwnChar;

protected://variables

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "AI")
	FPointLocation PointLocation;

protected://functions
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetPointLocation();

	FORCEINLINE AEnemyAIController* GetOwnController() const { return OwnController; }

	FORCEINLINE AEnemyCharacter* GetOwnCharacter() const { return OwnChar; }

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCharterBeginOverlapAttackSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnCharterEndOverlapAttackSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE  UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	UFUNCTION(BlueprintCallable)
	void SetIsStunned(const bool Stun);

};
