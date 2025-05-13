// Fill out your copyright notice in the Description page of Project Settings.
#include "Utilities/Components/PartyManagerComponent.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Actors/PlayerControllers/GamePlayerController.h"
#include "AI/AIControllers/PartyMemberAIController.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPartyManagerComponent::UPartyManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

// Called when the game starts
void UPartyManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UPartyManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPartyManagerComponent::SpawnParty()
{
	gamePlayerController = Cast<AGamePlayerController>(GetOwner());
	if (!gamePlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to AGamePlayerController in UPartyManagerComponent::SpawnParty!"));
		return;
	}

	// Set spawn information
	spawnLocation = FVector(-1450, -100, 100);
	offset = FVector(0, 100, 0); //Offset for next spawned character
	FRotator spawnRotation = FRotator::ZeroRotator;

	// Spawn party
	for (int i = 0; i < partyMembersToSpawn.Num(); i++)
	{
		if (partyMembersToSpawn[i])
		{
			// Spawn character
			APlayableCharacter* newCharacter = GetWorld()->SpawnActor<APlayableCharacter>(partyMembersToSpawn[i], spawnLocation + offset * i, spawnRotation);
			if (newCharacter)
			{
				// Add to party members array
				partyMembers.Add(newCharacter);

				// Possess first party member
				if (i == 0)
				{
					currentlyControlledPartyMemberIndex = i;
					newCharacter->playerController = gamePlayerController;
					gamePlayerController->Possess(newCharacter);
					APartyMemberAIController* partyMemberAIController = GetWorld()->SpawnActor<APartyMemberAIController>(APartyMemberAIController::StaticClass());
					if (partyMemberAIController)
					{
						newCharacter->partyMemberController = partyMemberAIController;
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to spawn an APartyMemberAIController for player controlled character in UPartyManagerComponent::SpawnParty!"));
						return;
					}
				}
				else
				{
					APartyMemberAIController* partyMemberAIController = GetWorld()->SpawnActor<APartyMemberAIController>(APartyMemberAIController::StaticClass());
					if (partyMemberAIController)
					{
						newCharacter->playerController = gamePlayerController;
						newCharacter->partyMemberController = partyMemberAIController;
						partyMemberAIController->Possess(newCharacter);
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to spawn an APartyMemberAIController for unpossessed character in UPartyManagerComponent::SpawnParty!"));
						return;
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to spawn actor at index %d"), i);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("party member to spawn is NULL at index %d"), i);
		}
	}
}

void UPartyManagerComponent::SwitchNextCharacter()
{
	gamePlayerController = Cast<AGamePlayerController>(GetOwner());
	if (!gamePlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to AGamePlayerController in UPartyManagerComponent::SwitchNextCharacter!"));
		return;
	}

	if (partyMembers.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No party members available in UPartyManagerComponent::SwitchNextCharacter!"));
		return;
	}

	int previouslyControlledPartyMemberIndex = currentlyControlledPartyMemberIndex;
	currentlyControlledPartyMemberIndex = (currentlyControlledPartyMemberIndex + 1) % partyMembers.Num();
	if (partyMembers.IsValidIndex(currentlyControlledPartyMemberIndex))
	{
		gamePlayerController->Possess(partyMembers[currentlyControlledPartyMemberIndex]);
		APartyMemberAIController* partyMemberAIController = partyMembers[previouslyControlledPartyMemberIndex]->partyMemberController;
		if (!partyMemberAIController)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get APartyMemberAIController from previously controlled character in UPartyManagerComponent::SwitchNextCharacter!"));
			return;
		}

		partyMemberAIController->Possess(partyMembers[previouslyControlledPartyMemberIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Index %d is not a valid index of partyMembers array - UPartyManagerComponent::SwitchNextCharacter"), currentlyControlledPartyMemberIndex);
		return;
	}
}

void UPartyManagerComponent::SwitchPreviousCharacter()
{
	gamePlayerController = Cast<AGamePlayerController>(GetOwner());
	if (!gamePlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to AGamePlayerController in UPartyManagerComponent::SwitchPreviousCharacter!"));
		return;
	}

	if (partyMembers.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No party members available in UPartyManagerComponent::SwitchPreviousCharacter!"));
		return;
	}

	int previouslyControlledPartyMemberIndex = currentlyControlledPartyMemberIndex;
	currentlyControlledPartyMemberIndex = (currentlyControlledPartyMemberIndex - 1 + partyMembers.Num()) % partyMembers.Num();
	if (partyMembers.IsValidIndex(currentlyControlledPartyMemberIndex))
	{
		gamePlayerController->Possess(partyMembers[currentlyControlledPartyMemberIndex]);
		APartyMemberAIController* partyMemberAIController = partyMembers[previouslyControlledPartyMemberIndex]->partyMemberController;
		if (!partyMemberAIController)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get APartyMemberAIController from previously controlled character in UPartyManagerComponent::SwitchPreviousCharacter!"));
			return;
		}

		partyMemberAIController->Possess(partyMembers[previouslyControlledPartyMemberIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Index %d is not a valid index of partyMembers array - UPartyManagerComponent::SwitchPreviousCharacter"), currentlyControlledPartyMemberIndex);
		return;
	}
}