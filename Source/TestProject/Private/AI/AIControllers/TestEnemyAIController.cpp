// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/AIControllers/TestEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

// AICONTROLLER IS USED TO RUN BEHAVIOR TREES AND MANAGE BLACKBOARDS

ATestEnemyAIController::ATestEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	bbComponent = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	btComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}

void ATestEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATestEnemyAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	/*AActor* TargetActor = Cast<AActor>(bbComponent->GetValueAsObject("TargetActor"));
	if (TargetActor)
		SetFocus(TargetActor); // Makes AI constantly rotate toward the player*/
}

void ATestEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!InPawn)
		return;

	if (btAsset && UseBlackboard(btAsset->BlackboardAsset, bbComponent))
	{
		InitializeBlackboard();
		RunBehaviorTree(btAsset);
	}
}

void ATestEnemyAIController::InitializeBlackboard()
{
	if (btAsset && btAsset->BlackboardAsset)
	{
		bbComponent->InitializeBlackboard(*btAsset->BlackboardAsset);
	}
}

void ATestEnemyAIController::SetTarget(AActor* target)
{
	if (bbComponent)
	{
		bbComponent->SetValueAsObject("TargetActor", target);
	}
}