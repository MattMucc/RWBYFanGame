// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AuraComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UAuraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAuraComponent();

	// Player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class APlayableCharacter* owningCharacter;

	// Aura
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Aura)
	float aura;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Aura)
	float maxAura;

	// Aura Regeneration
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AuraRegeneration)
	float regenRateAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AuraRegeneration)
	float regenPeriodicRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AuraRegeneration)
	float regenStartDelay;

	// Timers
	FTimerHandle regenTimer;
	FTimerHandle regenDelayTimer;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	bool GetOwningCharacter();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void RegenAura();
};