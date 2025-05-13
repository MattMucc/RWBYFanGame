// Fill out your copyright notice in the Description page of Project Settings.
#include "Utilities/Components/AuraComponent.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Components/ProgressBar.h"
#include "Widgets/CharacterInformation.h"

// Sets default values for this component's properties
UAuraComponent::UAuraComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// Aura
	maxAura = 100.0f;
	aura = maxAura;

	// Aura Regeneration
	regenRateAmount = 0.1f; // The amount regenerated every tick
	regenPeriodicRate = 0.01f; // Interval in which the aura regenerates
	regenStartDelay = 3.0f; // X second delay before regenerating aura
}

// Called when the game starts
void UAuraComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve APlayableCharacter in UAuraComponent::BeginPlay!"));
		return;
	}
}

bool UAuraComponent::GetOwningCharacter()
{
	owningCharacter = Cast<APlayableCharacter>(GetOwner());
	if (!owningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve APlayableCharacter in UAuraComponent::GetOwningCharacter!"));
		return false;
	}

	return true;
}

// Called every frame
void UAuraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAuraComponent::RegenAura()
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve APlayableCharacter in UAuraComponent::RegenAura!"));
		return;
	}

	aura += regenRateAmount;
	if (aura >= maxAura)
	{
		aura = FMath::Clamp(aura, 0.0f, maxAura);
		owningCharacter->characterInformationWidget->UpdateHealth();
		return;
	}

	owningCharacter->characterInformationWidget->UpdateHealth();
	owningCharacter->characterInformationWidget->auraDamageGauge->SetPercent(owningCharacter->characterInformationWidget->auraGauge->Percent);
	GetWorld()->GetTimerManager().SetTimer(regenTimer, this, &UAuraComponent::RegenAura, regenPeriodicRate, false);
}