// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterInformation.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UCharacterInformation : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& myGemoetry, float deltaTime) override;

	// Owning Character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class APlayableCharacter* owningCharacter;

	// Background
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Info, meta = (BindWidget))
	class UImage* background;

	// Name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Info, meta = (BindWidget))
	class UTextBlock* name;

	// Fire Mode Text
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FireModes, meta = (BindWidget))
	class UHorizontalBox* fireModesBox;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FireModes)
	class UTextBlock* fireModeText;
	
	// Ammo Text
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ammo, meta = (BindWidget))
	class UTextBlock* currentAmmo;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ammo, meta = (BindWidget))
	class UTextBlock* maxAmmo;

	// Ammo Icon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ammo, meta = (BindWidget))
	class UImage* ammoIcon;

	// Aura Gauges
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Aura, meta = (BindWidget))
	class UProgressBar* auraDamageGauge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Aura, meta = (BindWidget))
	class UProgressBar* auraGauge;

	// Aura Text
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Aura, meta = (BindWidget))
	class UTextBlock* currentAura;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Aura, meta = (BindWidget))
	class UTextBlock* maxAura;

	// Aura Icon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Aura, meta = (BindWidget))
	class UImage* auraIcon;

	// Health Gauges
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health, meta = (BindWidget))
	class UProgressBar* healthDamageGauge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health, meta = (BindWidget))
	class UProgressBar* healthGauge;

	// Health Text
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health, meta = (BindWidget))
	class UTextBlock* currentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health, meta = (BindWidget))
	class UTextBlock* maxHealth;

	// Health Icon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Health, meta = (BindWidget))
	class UImage* healthIcon;

	// Ultimate Gauge
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ultimate, meta = (BindWidget))
	class UProgressBar* ultimateGauge;

	// Ultimate Text
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ultimate, meta = (BindWidget))
	class UTextBlock* currentUltimate;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ultimate, meta = (BindWidget))
	class UTextBlock* maxUltimate;

	// Ultimate Icon
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Ultimate, meta = (BindWidget))
	class UImage* ultimateIcon;

	// Aura Damage Gauge Decay
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AuraDamageGaugeDecay)
	float auraDamageGaugeDecayRateAmount = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AuraDamageGaugeDecay)
	float auraDamageGaugeDecayPeriodicRate = 0.01f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AuraDamageGaugeDecay)
	float auraDamageGaugeDecayStartDelay = 1.0f;

	// Aura Damage Gauge Timers
	FTimerHandle auraDamageGaugeDecayTimer;
	FTimerHandle auraDamageGaugeDecayDelayTimer;

	// Health Damage Gauge Decay
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HealthDamageGaugeDecay)
	float healthDamageGaugeDecayRateAmount = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HealthDamageGaugeDecay)
	float healthDamageGaugeDecayPeriodicRate = 0.01f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = HealthDamageGaugeDecay)
	float healthDamageGaugeDecayStartDelay = 1.0f;

	// Health Damage Gauge Timers
	FTimerHandle healthDamageGaugeDecayTimer;
	FTimerHandle healthDamageGaugeDecayDelayTimer;

	UFUNCTION()
	void InitializeCharacterInformation();
	UFUNCTION()
	void UpdateHealth();
	UFUNCTION()
	void UpdateFireMode();
	UFUNCTION()
	void UpdateAmmo();
	UFUNCTION()
	void UpdateBackground(bool isPlayerControlled);
	UFUNCTION()
	void DecayAuraDamageGauge();
	UFUNCTION()
	void DecayHealthDamageGauge();
};