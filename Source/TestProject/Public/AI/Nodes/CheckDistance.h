// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckDistance.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UCheckDistance : public UBTService
{
	GENERATED_BODY()
	
public:
	UCheckDistance();

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, Category = "AI")
    float maxChaseDistance = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    float stopDistance = 150.0f;
};