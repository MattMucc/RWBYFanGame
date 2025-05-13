// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EFireMode
{
	Single,
	Burst,
	FullAuto,
	Lazer,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();

	// Player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class APlayableCharacter* owningCharacter;

	// Ammo
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ammo)
	float ammo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ammo)
	float maxAmmo;

	// Projectile Rates
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Rates)
	float singleShotRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Rates)
	float burstShotRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ammo)
	float burstDelay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Ammo)
	float bulletsPerBurst;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Rates)
	float bulletsPerBurstRemaining;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Rates)
	float fullAutoRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Rates)
	float lazerRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Rates)
	float reloadTime;

	FTimerHandle shootRateTimer;
	FTimerHandle reloadTimer;

	// Fire Modes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FireModes)
	int currentFireModeIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FireModes)
	EFireMode currentFireMode;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FireModes)
	TArray<EFireMode> availableFireModes;

	// Damage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Damage)
	float meleeDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Damage)
	float projectileDamage;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	bool GetOwningCharacter();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void UseAmmo();
	UFUNCTION()
	void EndShoot();
	UFUNCTION()
	void SingleShot();
	UFUNCTION()
	void BurstShot();
	UFUNCTION()
	void FullAuto();
	UFUNCTION()
	void Lazer();
	UFUNCTION()
	void Reload();
	UFUNCTION()
	void TryReload();
	UFUNCTION()
	void TryShoot();
	UFUNCTION()
	void ChangeFireMode();
};