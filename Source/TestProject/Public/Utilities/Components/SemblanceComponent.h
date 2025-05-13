// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SemblanceComponent.generated.h"

UENUM(BlueprintType)
enum class EDashType
{
	Ammo,
	Semblance,
	Physical // Jaune has to physically roll or dash since he has no ammo and no semblance-related way to dash
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API USemblanceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USemblanceComponent();

	// Player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class APlayableCharacter* owningCharacter;

	// Dash
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Dash)
	EDashType dashType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Dash)
	float dashCost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Dash)
	float dashStrength;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Dash)
	float dashDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Dash)
	float dashCooldown;

	FTimerHandle dashTimer;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	bool GetOwningCharacter();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void ResetDash();
	UFUNCTION()
	void EndDash();
	UFUNCTION()
	void StartDash();
};