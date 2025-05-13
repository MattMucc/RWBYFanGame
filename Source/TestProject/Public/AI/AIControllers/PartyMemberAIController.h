// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PartyMemberAIController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API APartyMemberAIController : public AAIController
{
	GENERATED_BODY()

public:
	APartyMemberAIController();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void InitializeBlackboard();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)
	class UBehaviorTree* btAsset;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UBlackboardComponent* bbComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class UBehaviorTreeComponent* btComponent;
};