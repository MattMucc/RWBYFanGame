// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PartyManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UPartyManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPartyManagerComponent();

	// Player Controller
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Controllers)
	class AGamePlayerController* gamePlayerController;

	// Party Information
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Party)
	TArray<TSubclassOf<class APlayableCharacter>> partyMembersToSpawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Party)
	TArray<APlayableCharacter*> partyMembers;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Party)
	int currentlyControlledPartyMemberIndex;

	// Spawn Information
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Spawn)
	FVector spawnLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Spawn)
	FVector offset;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SpawnParty();
	UFUNCTION()
	void SwitchNextCharacter();
	UFUNCTION()
	void SwitchPreviousCharacter();

};