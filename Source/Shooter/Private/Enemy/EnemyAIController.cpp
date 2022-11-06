// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyCharacter.h"
#include "Enemy/AIComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
	check(BlackboardComponent);
	check(BehaviorTreeComponent);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!InPawn) return;

	const auto Enemy = Cast<AEnemyCharacter>(InPawn);
	if (!Enemy)return;
	const auto Component = Cast<UAIComponent>(Enemy->GetComponentByClass(UAIComponent::StaticClass()));
	if (!Component || !Component->GetBehaviorTree())return;
	BlackboardComponent->InitializeBlackboard(*Component->GetBehaviorTree()->GetBlackboardAsset());
}
