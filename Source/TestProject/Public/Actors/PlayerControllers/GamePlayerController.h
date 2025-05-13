// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGamePlayerController();

	virtual void SetupInputComponent() override;

	// Player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	class APlayableCharacter* currentCharacter;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class UPartyManagerComponent* partyManagerComponent;

	// Player Hud Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widgets)
	TSubclassOf<UUserWidget> playerHudWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widgets)
	class UPlayerHud* playerHud;

	// Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* defaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* switchNextCharacterAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* switchPreviousCharacterAction;

	// Getters
	UFUNCTION()
	APlayableCharacter* GetControlledPlayableCharacter() const;

protected:
	virtual void BeginPlay() override;
};