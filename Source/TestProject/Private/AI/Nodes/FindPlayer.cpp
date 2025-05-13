// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/Nodes/FindPlayer.h"
#include "AI/AIControllers/TestEnemyAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"

UFindPlayer::UFindPlayer()
{
	NodeName = "Find Player";
}

EBTNodeResult::Type UFindPlayer::ExecuteTask(UBehaviorTreeComponent& btComponentOwner, uint8* nodeMemory)
{
	ATestEnemyAIController* aiController = Cast<ATestEnemyAIController>(btComponentOwner.GetAIOwner());
	if (!aiController) return EBTNodeResult::Failed;

	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!playerPawn) return EBTNodeResult::Failed;

	aiController->SetTarget(playerPawn);
	return EBTNodeResult::Succeeded;
}