// Fill out your copyright notice in the Description page of Project Settings.
#include "Utilities/Components/HealthComponent.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Actors/PlayerControllers/GamePlayerController.h"
#include "Components/ProgressBar.h"
#include "Utilities/Components/AuraComponent.h"
#include "Widgets/CharacterInformation.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// Health
	maxHealth = 100.0f;
	health = maxHealth;

	// Health Regeneration
	regenRateAmount = 0.1f; // The amount regenerated every tick
	regenPeriodicRate = 0.01f; // Interval in which the health regenerates
	regenStartDelay = 3.0f; // X second delay before regenerating health
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve APlayableCharacter in UHealthComponent::BeginPlay!"));
		return;
	}
}

bool UHealthComponent::GetOwningCharacter()
{
	owningCharacter = Cast<APlayableCharacter>(GetOwner());
	if (!owningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve APlayableCharacter in UHealthComponent::GetOwningCharacter!"));
		return false;
	}

	return true;
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::TakeDamage(float amount)
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve APlayableCharacter in UHealthComponent::TakeDamage!"));
		return;
	}

	UAuraComponent* auraComponent = owningCharacter->FindComponentByClass<UAuraComponent>();
	if (!auraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UAuraComponent in UHealthComponent::TakeDamage!"));
		return;
	}

	UCharacterInformation* characterInformation = owningCharacter->characterInformationWidget;
	if (!characterInformation)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retreive UCharacterInformation in UHealthComponent::TakeDamage!"));
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(auraComponent->regenTimer);
	auraComponent->aura -= amount;
	if (auraComponent->aura < 0)
	{
		float remainingDamage = -auraComponent->aura;
		auraComponent->aura = FMath::Clamp(auraComponent->aura, 0.0f, auraComponent->maxAura);
		health -= remainingDamage;
		health = FMath::Clamp(health, 0.0f, maxHealth);
		GetWorld()->GetTimerManager().SetTimer(characterInformation->healthDamageGaugeDecayDelayTimer, characterInformation, &UCharacterInformation::DecayHealthDamageGauge, characterInformation->healthDamageGaugeDecayStartDelay, false);
	}

	characterInformation->UpdateHealth();
	GetWorld()->GetTimerManager().SetTimer(characterInformation->auraDamageGaugeDecayDelayTimer, characterInformation, &UCharacterInformation::DecayAuraDamageGauge, characterInformation->auraDamageGaugeDecayStartDelay, false);
}

void UHealthComponent::RegenHealth()
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve APlayableCharacter in UHealthComponent::RegenHealth!"));
		return;
	}

	health += regenRateAmount;
	if (health >= maxHealth)
	{
		health = FMath::Clamp(health, 0.0f, maxHealth);
		owningCharacter->characterInformationWidget->UpdateHealth();
		return;
	}

	owningCharacter->characterInformationWidget->UpdateHealth();
	owningCharacter->characterInformationWidget->healthDamageGauge->SetPercent(owningCharacter->characterInformationWidget->healthGauge->Percent);
	GetWorld()->GetTimerManager().SetTimer(regenTimer, this, &UHealthComponent::RegenHealth, regenPeriodicRate, false);
}