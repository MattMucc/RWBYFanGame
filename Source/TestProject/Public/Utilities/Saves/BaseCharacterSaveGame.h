// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BaseCharacterSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UBaseCharacterSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	// Aura
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Aura)
	float aura;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Aura)
	float maxAura;

	// Health
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	float health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Health)
	float maxHealth;

	// Ultimate
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ultimate)
	float ultimate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ultimate)
	float maxUltimate;

	// Weapon
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	float ammo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	float maxAmmo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	int currentFireModeIndex;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	float meleeDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Weapon)
	float projectileDamage;
};