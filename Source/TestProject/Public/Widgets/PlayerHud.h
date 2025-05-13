// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHud.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UPlayerHud : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Character
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class AGamePlayerController* gamePlayerController;

	// Party
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components, meta = (BindWidget))
	class UVerticalBox* partyInfoBox;

protected:
	// Widgets
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Party)
	TArray<UCharacterInformation*> characterInfoWidgets;

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& myGemoetry, float deltaTime) override;

	UFUNCTION()
	void AddCharacterInformationToPartyInfoBox(UCharacterInformation* newCharacterInformation);
	UFUNCTION()
	void AddSpacerToPartyInfoBox();
};