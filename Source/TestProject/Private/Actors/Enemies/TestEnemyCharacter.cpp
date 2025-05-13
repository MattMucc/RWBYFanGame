// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/Enemies/TestEnemyCharacter.h"
#include "AI/AIControllers/TestEnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
ATestEnemyCharacter::ATestEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void ATestEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Create and set a new default AIController if none is set
	if (!AIControllerClass)
		AIControllerClass = ATestEnemyAIController::StaticClass();
}

// Called every frame
void ATestEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}