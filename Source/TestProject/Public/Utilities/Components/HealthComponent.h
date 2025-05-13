// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	// Player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class APlayableCharacter* owningCharacter;

	// Health
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	float health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	float maxHealth;

	// Health Regeneration
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HealthRegeneration)
	float regenRateAmount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HealthRegeneration)
	float regenPeriodicRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HealthRegeneration)
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
	void TakeDamage(float amount);
	UFUNCTION()
	void RegenHealth();
};