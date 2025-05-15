// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacter.generated.h"

UCLASS()
class TESTPROJECT_API APlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayableCharacter();

	// Info
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	FName characterName;

	// Player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class APlayerController* playerController;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	class APartyMemberAIController* partyMemberController;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UAuraComponent* auraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UHealthComponent* healthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class USemblanceComponent* semblanceComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UUltimateComponent* ultimateComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UWeaponComponent* weaponComponent;

	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class USpringArmComponent* springArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera)
	class UCameraComponent* cameraComponent;

	// Character Information Widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widgets)
	TSubclassOf<UUserWidget> characterInformationWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Widgets)
	class UCharacterInformation* characterInformationWidget;

	// Physics
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Physics)
	float gravityScale;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Physics)
	float groundFriction;

	// Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	float walkingMaxAcceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float airMaxAcceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	float walkingBrakingDeceleration;

	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector traditionalThirdPersonSpringArmLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	FVector overTheShoulderSpringArmLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float traditionalThirdPersonSpringArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float overTheShoulderSpringArmLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float springArmLerpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float horizontalSensitivityMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float verticalSensitivityMultiplier;

	// Vectors
	FVector lastMovementInput;

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	// Initialization Functions
	UFUNCTION()
	void CreateCharacterInformation();

	// Movement Mode Function
	UFUNCTION()
	void LerpSpringArmLength();
	UFUNCTION()
	void LerpSpringArmLocation();
	UFUNCTION()
	void CameraBasedMovement();
	UFUNCTION()
	void PlayerBasedMovement();
	UFUNCTION()
	void EnterOverTheShoulderMode();
	UFUNCTION()
	void ExitOverTheShoulderMode();

	// Save/Load Functions
	UFUNCTION()
	void SaveStats();
	UFUNCTION()
	void LoadStats();

	// Inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputMappingContext* defaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* moveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* lookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* jumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* aimAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* shootAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* reloadAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* changeFireModeAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	class UInputAction* dashAction;

	// Save/Load Instance
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Save/Load")
	class UBaseCharacterSaveGame* statsInstance;

	// Spring Arm Lerp
	FTimerHandle springArmLocationTimer;
	FTimerHandle springArmLengthTimer;
	FVector startSpringArmLocation;
	FVector targetSpringArmLocation;
	float startSpringArmLength;
	float targetSpringArmLength;
	float springArmLocationElpasedTime;
	float springArmLengthElapsedTime;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* newController) override;

	// Movement Functions
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	// Setters
	UFUNCTION()
	void SetInputMappingContext(UInputMappingContext* InputMappingContext);
};