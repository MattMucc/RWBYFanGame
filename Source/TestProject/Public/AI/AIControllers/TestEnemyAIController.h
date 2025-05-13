// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TestEnemyAIController.generated.h"

/**
 * AICONTROLLER IS USED TO RUN BEHAVIOR TREES AND MANAGE BLACKBOARDS
 */
UCLASS()
class TESTPROJECT_API ATestEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATestEnemyAIController();

	void SetTarget(AActor* target);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	void InitializeBlackboard();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	class UBehaviorTree* btAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UBlackboardComponent* bbComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UBehaviorTreeComponent* btComponent;
};