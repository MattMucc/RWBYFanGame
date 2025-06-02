// Fill out your copyright notice in the Description page of Project Settings.
#include "Widgets/CharacterInformation.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Utilities/Components/AuraComponent.h"
#include "Utilities/Components/HealthComponent.h"
#include "Utilities/Components/UltimateComponent.h"
#include "Utilities/Components/WeaponComponent.h"

void UCharacterInformation::NativeConstruct()
{
	Super::NativeConstruct();
	if (!owningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find the owning character in UCharacterInformation::NativeConstruct!"));
		return;
	}
}

void UCharacterInformation::NativeTick(const FGeometry& myGemoetry, float deltaTime)
{
	Super::NativeTick(myGemoetry, deltaTime);
}

void UCharacterInformation::InitializeCharacterInformation()
{
	UAuraComponent* auraComponent = owningCharacter->FindComponentByClass<UAuraComponent>();
	if (!auraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UAuraComponent in UCharacterInformation::InitializeCharacterInformation!"));
		return;
	}

	UHealthComponent* healthComponent = owningCharacter->FindComponentByClass<UHealthComponent>();
	if (!healthComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UHealthComponent in UCharacterInformation::InitializeCharacterInformation!"));
		return;
	}

	UUltimateComponent* ultimateComponent = owningCharacter->FindComponentByClass<UUltimateComponent>();
	if (!ultimateComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UUltimateComponent in UCharacterInformation::InitializeCharacterInformation!"));
		return;
	}

	UWeaponComponent* weaponComponent = owningCharacter->FindComponentByClass<UWeaponComponent>();
	if (!weaponComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UWeaponComponent in UCharacterInformation::InitializeCharacterInformation!"));
		return;
	}

	// Player
	name->SetText(FText::FromName(owningCharacter->characterName));

	// Aura
	auraGauge->SetPercent(auraComponent->aura / auraComponent->maxAura);
	auraDamageGauge->SetPercent(auraGauge->Percent);
	currentAura->SetText(FText::AsNumber(auraComponent->aura));
	maxAura->SetText(FText::AsNumber(auraComponent->maxAura));

	// Health
	healthGauge->SetPercent(healthComponent->health / healthComponent->maxHealth);
	healthDamageGauge->SetPercent(healthGauge->Percent);
	currentHealth->SetText(FText::AsNumber(healthComponent->health));
	maxHealth->SetText(FText::AsNumber(healthComponent->maxHealth));

	// Ultimate
	ultimateGauge->SetPercent(ultimateComponent->ultimate / ultimateComponent->maxUltimate);
	currentUltimate->SetText(FText::AsNumber(ultimateComponent->ultimate));
	maxUltimate->SetText(FText::AsNumber(ultimateComponent->maxUltimate));

	// Weapon
	currentAmmo->SetText(FText::AsNumber(weaponComponent->ammo));
	maxAmmo->SetText(FText::AsNumber(weaponComponent->maxAmmo));
	for (int i = 0; i <= weaponComponent->availableFireModes.Num() - 1; i++)
	{
		// Setting text
		fireModeText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		fireModeText->Font.Size = 6;
		fireModeText->SetJustification(ETextJustify::Center);
		if (i != weaponComponent->currentFireModeIndex)
			fireModeText->SetColorAndOpacity(FSlateColor(FLinearColor(0.33f, 0.33f, 0.33f, 1.0f)));

		if (weaponComponent->availableFireModes[i] == EFireMode::Single)
			fireModeText->SetText(FText::FromName("Single"));
		else if (weaponComponent->availableFireModes[i] == EFireMode::Burst)
			fireModeText->SetText(FText::FromName("Burst"));
		else if (weaponComponent->availableFireModes[i] == EFireMode::FullAuto)
			fireModeText->SetText(FText::FromName("Full Auto"));
		else if (weaponComponent->availableFireModes[i] == EFireMode::Lazer)
			fireModeText->SetText(FText::FromName("Lazer"));

		// Add to fire modes box to set slot
		fireModesBox->AddChildToHorizontalBox(fireModeText);

		// Setting slot
		UHorizontalBoxSlot* slot = Cast<UHorizontalBoxSlot>(fireModeText->Slot);
		if (!slot)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast to UHorizontalBoxSlot in UCharacterInformation::InitializeCharacterInformation!"));
			return;
		}

		slot->Size.SizeRule = ESlateSizeRule::Fill;
		slot->HorizontalAlignment = EHorizontalAlignment::HAlign_Center;
		slot->VerticalAlignment = EVerticalAlignment::VAlign_Center;
		slot->SetPadding(FMargin(0, 1.5f, 0, 0));
	}
}

void UCharacterInformation::UpdateHealth()
{
	UAuraComponent* auraComponent = owningCharacter->FindComponentByClass<UAuraComponent>();
	if (!auraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UAuraComponent in UCharacterInformation::UpdateHealth!"));
		return;
	}

	UHealthComponent* healthComponent = owningCharacter->FindComponentByClass<UHealthComponent>();
	if (!healthComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UHealthComponent in UCharacterInformation::UpdateHealth!"));
		return;
	}

	// Aura
	auraGauge->SetPercent(auraComponent->aura / auraComponent->maxAura);
	currentAura->SetText(FText::AsNumber(FMath::FloorToInt(auraComponent->aura)));
	maxAura->SetText(FText::AsNumber(FMath::FloorToInt(auraComponent->maxAura)));

	// Health
	healthGauge->SetPercent(healthComponent->health / healthComponent->maxHealth);
	currentHealth->SetText(FText::AsNumber(FMath::FloorToInt(healthComponent->health)));
	maxHealth->SetText(FText::AsNumber(FMath::FloorToInt(healthComponent->maxHealth)));
}

void UCharacterInformation::UpdateFireMode()
{
	UWeaponComponent* weaponComponent = owningCharacter->FindComponentByClass<UWeaponComponent>();
	if (!weaponComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UWeaponComponent in UCharacterInformation::InitializeCharacterInformation!"));
		return;
	}

	for (int i = 0; i <= fireModesBox->GetChildrenCount() - 1; i++)
	{
		fireModeText = Cast<UTextBlock>(fireModesBox->GetChildAt(i));
		if (!fireModeText)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast to UTextBlock in UCharacterInformation::UpdateFireMode!"));
			return;
		}

		if (i == weaponComponent->currentFireModeIndex)
			fireModeText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f)));
		else if (i != weaponComponent->currentFireModeIndex)
			fireModeText->SetColorAndOpacity(FSlateColor(FLinearColor(0.33f, 0.33f, 0.33f, 1.0f)));
	}
}

void UCharacterInformation::UpdateAmmo()
{
	UWeaponComponent* weaponComponent = owningCharacter->FindComponentByClass<UWeaponComponent>();
	if (!weaponComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UWeaponComponent in UCharacterInformation::UpdateAmmo!"));
		return;
	}

	currentAmmo->SetText(FText::AsNumber(weaponComponent->ammo));
	maxAmmo->SetText(FText::AsNumber(weaponComponent->maxAmmo));
}

void UCharacterInformation::UpdateBackground(bool isPlayerControlled)
{
	if (isPlayerControlled) // Blue
	{
		background->SetColorAndOpacity(FLinearColor(0.0f, 0.055f, 0.55f, 0.7f));
	}
	else // Grey
	{
		background->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 0.7f));
	}
}

void UCharacterInformation::DecayAuraDamageGauge()
{
	UAuraComponent* auraComponent = owningCharacter->FindComponentByClass<UAuraComponent>();
	if (!auraComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UAuraComponent in UCharacterInformation::DecayAuraDamageGauge!"));
		return;
	}

	auraDamageGauge->SetPercent(auraDamageGauge->Percent -= auraDamageGaugeDecayRateAmount);
	if (auraDamageGauge->Percent <= auraGauge->Percent)
	{
		auraDamageGauge->SetPercent(auraGauge->Percent);
		GetWorld()->GetTimerManager().SetTimer(auraComponent->regenDelayTimer, auraComponent, &UAuraComponent::RegenAura, auraComponent->regenStartDelay, false);
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(auraDamageGaugeDecayTimer, this, &UCharacterInformation::DecayAuraDamageGauge, auraDamageGaugeDecayPeriodicRate, false);
}

void UCharacterInformation::DecayHealthDamageGauge()
{
	UHealthComponent* healthComponent = owningCharacter->FindComponentByClass<UHealthComponent>();
	if (!healthComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to retrieve UHealthComponent in UCharacterInformation::DecayHealthDamageGauge!"));
		return;
	}

	healthDamageGauge->SetPercent(healthDamageGauge->Percent -= healthDamageGaugeDecayRateAmount);
	if (healthDamageGauge->Percent <= healthGauge->Percent)
	{
		healthDamageGauge->SetPercent(healthGauge->Percent);
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(healthDamageGaugeDecayTimer, this, &UCharacterInformation::DecayHealthDamageGauge, healthDamageGaugeDecayPeriodicRate, false);
}