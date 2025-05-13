// Fill out your copyright notice in the Description page of Project Settings.
#include "AI/Nodes/CheckDistance.h"
#include "AI/AIControllers/TestEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"

UCheckDistance::UCheckDistance()
{
	NodeName = "Check Distance";
}

void UCheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* aiController = OwnerComp.GetAIOwner();
    if (!aiController) return;

    UBlackboardComponent* blackboardComp = aiController->GetBlackboardComponent();
    if (!blackboardComp) return;

    AActor* playerActor = Cast<AActor>(blackboardComp->GetValueAsObject("TargetActor"));
    APawn* controlledPawn = aiController->GetPawn();

    if (!playerActor || !controlledPawn) return;

    float distance = FVector::Dist(playerActor->GetActorLocation(), controlledPawn->GetActorLocation());
    bool shouldMove = (distance > stopDistance && distance <= maxChaseDistance);
    blackboardComp->SetValueAsBool("CanMoveToTarget", shouldMove);
}