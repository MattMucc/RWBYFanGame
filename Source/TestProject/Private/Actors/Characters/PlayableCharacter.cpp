// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/Characters/PlayableCharacter.h"
#include "Actors/PlayerControllers/GamePlayerController.h"
#include "AI/AIControllers/PartyMemberAIController.h"
#include "AIController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/InputComponent.h"
#include "Components/VerticalBox.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/Components/AuraComponent.h"
#include "Utilities/Components/HealthComponent.h"
#include "Utilities/Components/SemblanceComponent.h"
#include "Utilities/Components/UltimateComponent.h"
#include "Utilities/Components/WeaponComponent.h"
#include "Utilities/Saves/BaseCharacterSaveGame.h"
#include "Widgets/CharacterInformation.h"
#include "Widgets/PlayerHud.h"

// Sets default values
APlayableCharacter::APlayableCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	springArmComponent->SetupAttachment(RootComponent);
	springArmComponent->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	springArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	cameraComponent->SetupAttachment(springArmComponent, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	cameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create character components
	auraComponent = CreateDefaultSubobject<UAuraComponent>("AuraComponent");
	healthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	semblanceComponent = CreateDefaultSubobject<USemblanceComponent>("SemblanceComponent");
	ultimateComponent = CreateDefaultSubobject<UUltimateComponent>("UltimateComponent");
	weaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->GroundFriction = 8.f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->MaxAcceleration = 1500;
	GetCharacterMovement()->BrakingDecelerationWalking = 10000.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Setting variable values
	horizontalSensitivityMultiplier = 1.f;
	verticalSensitivityMultiplier = 1.f;

	groundFriction = GetCharacterMovement()->GroundFriction;
	gravityScale = GetCharacterMovement()->GravityScale;
	walkingMaxAcceleration = GetCharacterMovement()->MaxAcceleration;
	airMaxAcceleration = 10000;
	walkingBrakingDeceleration = GetCharacterMovement()->BrakingDecelerationWalking;

	SetActorLabel(characterName.ToString());
}

// Called when the game starts or when spawned
void APlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!playerController)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get APlayerController in APlayableCharacter::BeginPlay!"));
		return;
	}

	groundFriction = GetCharacterMovement()->GroundFriction;
	gravityScale = GetCharacterMovement()->GravityScale;
	walkingMaxAcceleration = GetCharacterMovement()->MaxAcceleration;
	walkingBrakingDeceleration = GetCharacterMovement()->BrakingDecelerationWalking;

	LoadStats();
	CreateCharacterInformation();
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
	{
		subsystem->AddMappingContext(defaultMappingContext, 0); //Default mapping context is set in AGamePlayerControler::BeginPlay
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get UEnhancedInputLocalPlayerSubsystem in APlayableCharacter::BeginPlay!"));
		return;
	}
}

// Called every frame
void APlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!weaponComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get UWeaponComponent in APlayableCharacter::SetupPlayerInputComponent!"));
		return;
	}

	if (!semblanceComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get USemblanceComponent in APlayableCharacter::SetupPlayerInputComponent!"));
		return;
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		// Player Controls
		EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayableCharacter::Move);
		EnhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayableCharacter::Look);

		// Jumping
		EnhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Triggered, this, &APlayableCharacter::Jump);
		EnhancedInputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &APlayableCharacter::StopJumping);

		// Shooting
		//EnhancedInputComponent->BindAction(aimAction, ETriggerEvent::Triggered, weaponComponent, &UWeaponComponent::ScopeIn);
		//EnhancedInputComponent->BindAction(aimAction, ETriggerEvent::Completed, weaponComponent, &UWeaponComponent::ScopeOut);
		EnhancedInputComponent->BindAction(shootAction, ETriggerEvent::Triggered, weaponComponent, &UWeaponComponent::TryShoot);
		EnhancedInputComponent->BindAction(reloadAction, ETriggerEvent::Triggered, weaponComponent, &UWeaponComponent::TryReload);

		// Changing Fire Mode
		EnhancedInputComponent->BindAction(changeFireModeAction, ETriggerEvent::Triggered, weaponComponent, &UWeaponComponent::ChangeFireMode);

		// Dashing
		EnhancedInputComponent->BindAction(dashAction, ETriggerEvent::Triggered, semblanceComponent, &USemblanceComponent::StartDash);
	}
}

void APlayableCharacter::PossessedBy(AController* newController)
{
	Super::PossessedBy(newController);
	if (AGamePlayerController* gamePlayerController = Cast<AGamePlayerController>(newController))
	{
		playerController = gamePlayerController;
		characterInformationWidget->UpdateBackground(true);
	}

	if (APartyMemberAIController* partyMemberPlayerController = Cast<APartyMemberAIController>(newController))
	{
		partyMemberController = partyMemberPlayerController;
		characterInformationWidget->UpdateBackground(false);
	}
}

void APlayableCharacter::CreateCharacterInformation()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	characterInformationWidget = CreateWidget<UCharacterInformation>(controller, characterInformationWidgetClass);
	characterInformationWidget->owningCharacter = this;
	characterInformationWidget->InitializeCharacterInformation();
	AGamePlayerController* gamePlayerController = Cast<AGamePlayerController>(controller);
	if (gamePlayerController)
	{
		gamePlayerController->playerHud->AddCharacterInformationToPartyInfoBox(characterInformationWidget);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to AGamePlayerController in APlayableCharacter::CreateCharacterInformation!"));
		return;
	}
}

void APlayableCharacter::SaveStats()
{
	statsInstance = Cast<UBaseCharacterSaveGame>(UGameplayStatics::CreateSaveGameObject(UBaseCharacterSaveGame::StaticClass()));
	if (!statsInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to UBaseCharacterSaveGame in APlayableCharacter::SaveStats!"));
		return;
	}

	statsInstance->aura = auraComponent->aura;
	statsInstance->maxAura = auraComponent->maxAura;
	statsInstance->health = healthComponent->health;
	statsInstance->maxHealth = healthComponent->maxHealth;
	statsInstance->ultimate = ultimateComponent->ultimate;
	statsInstance->maxUltimate = ultimateComponent->maxUltimate;
	statsInstance->ammo = weaponComponent->ammo;
	statsInstance->maxAmmo = weaponComponent->maxAmmo;
	statsInstance->currentFireModeIndex = weaponComponent->currentFireModeIndex;
	statsInstance->meleeDamage = weaponComponent->meleeDamage;
	statsInstance->projectileDamage = weaponComponent->projectileDamage;
	UGameplayStatics::SaveGameToSlot(statsInstance, TEXT("BaseCharacterStats"), 0);
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Base Character Stats Saved!"));
}

void APlayableCharacter::LoadStats()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("BaseCharacterStats"), 0))
	{
		UE_LOG(LogTemp, Error, TEXT("No save file exists to load yet in APlayableCharacter::LoadStats!"));
		return;
	}

	statsInstance = Cast<UBaseCharacterSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("BaseCharacterStats"), 0));
	if (!statsInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load previously saved instance in APlayableCharacter::LoadStats!"));
		return;
	}

	auraComponent->aura = statsInstance->aura;
	auraComponent->maxAura = statsInstance->maxAmmo;
	healthComponent->health = statsInstance->health;
	healthComponent->maxHealth = statsInstance->maxHealth;
	ultimateComponent->ultimate = statsInstance->ultimate;
	ultimateComponent->maxUltimate = statsInstance->maxUltimate;
	weaponComponent->ammo = statsInstance->ammo;
	weaponComponent->maxAmmo = statsInstance->maxAmmo;
	weaponComponent->currentFireModeIndex = statsInstance->currentFireModeIndex;
	weaponComponent->meleeDamage = statsInstance->meleeDamage;
	weaponComponent->projectileDamage = statsInstance->projectileDamage;
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Base Character Stats Loaded!"));
}

void APlayableCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D movementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0, rotation.Yaw, 0);

		// get forward vector
		const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

		// set the movement input
		lastMovementInput = (forwardDirection * movementVector.Y + rightDirection * movementVector.X).GetSafeNormal();

		// add movement 
		AddMovementInput(forwardDirection, movementVector.Y);
		AddMovementInput(rightDirection, movementVector.X);
	}
}

void APlayableCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D lookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(lookAxisVector.X * horizontalSensitivityMultiplier);
		AddControllerPitchInput(lookAxisVector.Y * verticalSensitivityMultiplier);
	}
}

void APlayableCharacter::Jump()
{
	if (ActorHasTag(FName("IsDashing")))
		return;

	Super::Jump();
	GetCharacterMovement()->MaxAcceleration = airMaxAcceleration;
}

void APlayableCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	GetCharacterMovement()->MaxAcceleration = walkingMaxAcceleration;
}

void APlayableCharacter::SetInputMappingContext(UInputMappingContext* InputMappingContext)
{
	defaultMappingContext = InputMappingContext;
}