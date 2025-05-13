// Fill out your copyright notice in the Description page of Project Settings.
#include "Widgets/PlayerHud.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Actors/PlayerControllers/GamePlayerController.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/VerticalBox.h"
#include "Components/Spacer.h"
#include "Widgets/CharacterInformation.h"

void UPlayerHud::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* controller = Cast<APlayerController>(GetOwningPlayer());
	if (controller)
	{
		AGamePlayerController* playerController = Cast<AGamePlayerController>(controller);
		if (playerController)
		{
			gamePlayerController = playerController;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast to AGamePlayerController in UPlayerHud::NativeConstruct!"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to APlayerController in UPlayerHud::NativeConstruct!"));
		return;
	}
}

void UPlayerHud::NativeTick(const FGeometry& myGemoetry, float deltaTime)
{
	Super::NativeTick(myGemoetry, deltaTime);
}

void UPlayerHud::AddCharacterInformationToPartyInfoBox(UCharacterInformation* newCharacterInformation)
{
	characterInfoWidgets.Add(newCharacterInformation);
	partyInfoBox->ClearChildren();
	for (int i = characterInfoWidgets.Num() - 1; i >= 0; i--)
	{
		partyInfoBox->AddChildToVerticalBox(characterInfoWidgets[i]);
		AddSpacerToPartyInfoBox();
	}
}

void UPlayerHud::AddSpacerToPartyInfoBox()
{
	USpacer* spacer = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
	if (spacer)
	{
		spacer->SetSize(FVector2D(1, 5));
		partyInfoBox->AddChildToVerticalBox(spacer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create USpacer in UPlayerHud::AddSpacerToPartyInfoBox!"));
		return;
	}
}