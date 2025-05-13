// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UFindPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UFindPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& btComponentOwner, uint8* nodeMemory) override;
};