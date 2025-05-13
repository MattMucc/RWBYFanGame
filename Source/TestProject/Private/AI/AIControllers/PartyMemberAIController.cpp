// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/AIControllers/PartyMemberAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

APartyMemberAIController::APartyMemberAIController()
{
	bbComponent = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	btComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}

void APartyMemberAIController::BeginPlay()
{
	Super::BeginPlay();
}

void APartyMemberAIController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void APartyMemberAIController::OnPossess(APawn* InPawn)
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

void APartyMemberAIController::InitializeBlackboard()
{
	if (btAsset && btAsset->BlackboardAsset)
	{
		bbComponent->InitializeBlackboard(*btAsset->BlackboardAsset);
	}
}