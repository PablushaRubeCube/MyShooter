// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AIComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"
#include "Components/SphereComponent.h"
//#include "BehaviorTree/BehaviorTree.h"

// Sets default values for this component's properties
UAIComponent::UAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UAIComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnChar = Cast<AEnemyCharacter>(GetOwner());
	check(GetOwnCharacter());
	OwnController = Cast<AEnemyAIController>(OwnChar->GetController());
	check(GetOwnController());
	check(GetOwnController()->GetBlackboardComponent());
	
	GetOwnCharacter()->GetAgroSphere()->OnComponentBeginOverlap.AddDynamic(this, &UAIComponent::OnComponentBeginOverlap);
	GetOwnCharacter()->GetAttackSphere()->OnComponentBeginOverlap.AddDynamic(this, &UAIComponent::OnCharterBeginOverlapAttackSphere);
	GetOwnCharacter()->GetAttackSphere()->OnComponentEndOverlap.AddDynamic(this, &UAIComponent::OnCharterEndOverlapAttackSphere);

	GetOwnController()->RunBehaviorTree(BehaviorTree);
	SetPointLocation();
}

void UAIComponent::SetPointLocation()
{
	for (auto i = 0; i < PointLocation.PointLocation.Num(); i++)
	{
		const auto PointDestination = UKismetMathLibrary::TransformLocation(
			GetOwnCharacter()->GetActorTransform(), PointLocation.PointLocation[i]);
		GetOwnController()->GetBlackboardComponent()->SetValueAsVector(PointLocation.LocationName[i], PointDestination);
	}	
}

void UAIComponent::SetIsStunned(const bool Stun)
{
	OwnController->GetBlackboardComponent()->SetValueAsBool("IsStunned", Stun);
}

void UAIComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto AgroChar = Cast<AShooterCharacter>(OtherActor);
	if (!AgroChar) return;
	GetOwnController()->GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), AgroChar);
}

void UAIComponent::OnCharterBeginOverlapAttackSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto AgroChar = Cast<AShooterCharacter>(OtherActor);
	if (!AgroChar) return;
	GetOwnController()->GetBlackboardComponent()->SetValueAsBool(TEXT("IsEnemyInAgroSphere"), true);
}

void UAIComponent::OnCharterEndOverlapAttackSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto AgroChar = Cast<AShooterCharacter>(OtherActor);
	if (!AgroChar) return;
	GetOwnController()->GetBlackboardComponent()->SetValueAsBool(TEXT("IsEnemyInAgroSphere"), false);
}




// Called every frame
void UAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

