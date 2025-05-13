// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UltimateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UUltimateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUltimateComponent();

	// Player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class APlayableCharacter* owningCharacter;

	// Ultimate
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ultimate)
	float ultimate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ultimate)
	float maxUltimate;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};