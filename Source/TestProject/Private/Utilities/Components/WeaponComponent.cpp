// Fill out your copyright notice in the Description page of Project Settings.
#include "Utilities/Components/WeaponComponent.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Widgets/CharacterInformation.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// Ammo
	maxAmmo = 25.0f;
	ammo = maxAmmo;

	// Rates
	singleShotRate = 1.0f;
	burstShotRate = 0.05f;
	burstDelay = 0.25f;
	bulletsPerBurst = 5;
	bulletsPerBurstRemaining = bulletsPerBurst;
	fullAutoRate = 0.25f;
	lazerRate = 0.1f;
	reloadTime = 3.0f;

	// Fire Modes
	currentFireModeIndex = 0;
	availableFireModes = { EFireMode::Single };

	// Damage
	meleeDamage = 5;
	projectileDamage = 1;
}

// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get owning character in UWeaponComponent::BeginPlay!"));
		return;
	}

	if (!availableFireModes.IsValidIndex(currentFireModeIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Current fire mode index is not a valid index in available fire modes - UWeaponComponent::BeginPlay"));
		return;
	}

	owningCharacter->Tags.Add(FName("CanShoot"));
	owningCharacter->Tags.Add(FName("CanReload"));
	currentFireMode = availableFireModes[currentFireModeIndex];
}

// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UWeaponComponent::GetOwningCharacter()
{
	owningCharacter = Cast<APlayableCharacter>(GetOwner());
	if (!owningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get owning character in UWeaponComponent::GetOwningCharacter!"));
		return false;
	}

	return true;
}

void UWeaponComponent::UseAmmo()
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get owning character in UWeaponComponent::UseAmmo!"));
		return;
	}

	ammo--;
	ammo = FMath::Clamp(ammo, 0, maxAmmo);
	owningCharacter->characterInformationWidget->UpdateAmmo();
}

void UWeaponComponent::EndShoot()
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get owning character in UWeaponComponent::EndShoot!"));
		return;
	}

	owningCharacter->Tags.Remove(FName("IsShooting"));
}

void UWeaponComponent::SingleShot()
{
	UseAmmo();
	GetWorld()->GetTimerManager().SetTimer(shootRateTimer, this, &UWeaponComponent::EndShoot, singleShotRate, false);
}

void UWeaponComponent::BurstShot()
{
	UseAmmo();
	bulletsPerBurstRemaining--;
	if (bulletsPerBurstRemaining <= 0)
	{
		GetWorld()->GetTimerManager().SetTimer(shootRateTimer, this, &UWeaponComponent::EndShoot, burstDelay, false);
		bulletsPerBurstRemaining = bulletsPerBurst;
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(shootRateTimer, this, &UWeaponComponent::EndShoot, burstShotRate, false);
	}
}

void UWeaponComponent::FullAuto()
{
	UseAmmo();
	GetWorld()->GetTimerManager().SetTimer(shootRateTimer, this, &UWeaponComponent::EndShoot, fullAutoRate, false);
}

void UWeaponComponent::Lazer()
{
	UseAmmo();
	GetWorld()->GetTimerManager().SetTimer(shootRateTimer, this, &UWeaponComponent::EndShoot, lazerRate, false);
}

void UWeaponComponent::Reload()
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get owning character in UWeaponComponent::Reload!"));
		return;
	}

	ammo = maxAmmo;
	owningCharacter->characterInformationWidget->UpdateAmmo();
	owningCharacter->Tags.Remove(FName("IsReloading"));
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Green, FString("Reloaded!"), true);
}

void UWeaponComponent::TryReload()
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get owning character in UWeaponComponent::TryReload!"));
		return;
	}

	if (owningCharacter->ActorHasTag(FName("IsShooting")) || owningCharacter->ActorHasTag(FName("IsReloading")))
		return;

	if (ammo == maxAmmo)
		return;

	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Yellow, FString("Reloading..."), true);
	owningCharacter->Tags.Add(FName("IsReloading"));
	GetWorld()->GetTimerManager().SetTimer(reloadTimer, this, &UWeaponComponent::Reload, reloadTime, false);
}

void UWeaponComponent::TryShoot()
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get owning character in UWeaponComponent::TryShoot!"));
		return;
	}

	if (owningCharacter->ActorHasTag(FName("IsShooting")) || owningCharacter->ActorHasTag(FName("IsReloading")))
		return;

	if (ammo <= 0)
	{
		TryReload();
		return;
	}

	owningCharacter->Tags.Add(FName("IsShooting"));
	switch (currentFireMode)
	{
		case EFireMode::Single:
		{
			SingleShot();
			break;
		}

		case EFireMode::Burst:
		{
			BurstShot();
			break;
		}

		case EFireMode::FullAuto:
		{
			FullAuto();
			break;
		}

		case EFireMode::Lazer:
		{
			Lazer();
			break;
		}
	}
}

void UWeaponComponent::ChangeFireMode()
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get owning character in UWeaponComponent::ChangeFireMode!"));
		return;
	}

	if (owningCharacter->ActorHasTag(FName("IsShooting")))
		return;

	currentFireModeIndex++;
	if (availableFireModes.IsValidIndex(currentFireModeIndex))
	{
		currentFireMode = availableFireModes[currentFireModeIndex];
	}
	else
	{
		currentFireModeIndex = 0;
		if (availableFireModes.IsValidIndex(currentFireModeIndex))
		{
			currentFireMode = availableFireModes[currentFireModeIndex];
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid index [%d] in available fire modes array!"), currentFireMode);
			return;
		}
	}

	owningCharacter->characterInformationWidget->UpdateFireMode();
}