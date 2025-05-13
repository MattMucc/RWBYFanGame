// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/PlayerControllers/GamePlayerController.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Components/InputComponent.h"
#include "Components/VerticalBox.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Utilities/Components/PartyManagerComponent.h"
#include "Widgets/CharacterInformation.h"
#include "Widgets/PlayerHud.h"

AGamePlayerController::AGamePlayerController()
{
	partyManagerComponent = CreateDefaultSubobject<UPartyManagerComponent>("PartyManagerComponent");
}

void AGamePlayerController::BeginPlay()
{
	playerHud = CreateWidget<UPlayerHud>(this, playerHudWidgetClass, "PlayerHUD");
	playerHud->AddToViewport();

	partyManagerComponent->SpawnParty();
	currentCharacter = GetControlledPlayableCharacter();
	if (!currentCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get controlled APlayableCharacter in AGamePlayerController::BeginPlay!"));
		return;
	}

	currentCharacter->SetInputMappingContext(defaultMappingContext);
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (defaultMappingContext)
		{
			InputSubsystem->AddMappingContext(defaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("defaultMappingContext variable is NULL in AGamePlayerController::BeginPlay!"));
		}
	}
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(switchNextCharacterAction, ETriggerEvent::Triggered, partyManagerComponent, &UPartyManagerComponent::SwitchNextCharacter);
		EnhancedInputComponent->BindAction(switchPreviousCharacterAction, ETriggerEvent::Triggered, partyManagerComponent, &UPartyManagerComponent::SwitchPreviousCharacter);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to UEnhancedInputComponent in AGamePlayerController::SetupInputComponent!"));
	}
}

APlayableCharacter* AGamePlayerController::GetControlledPlayableCharacter() const
{
	APlayableCharacter* character = Cast<APlayableCharacter>(GetPawn());
	if (character)
	{
		return character;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get controlled APlayableCharacter in AGamePlayerController::GetControlledPlayableCharacter!"));
		return nullptr;
	}

	return nullptr;
}