// Fill out your copyright notice in the Description page of Project Settings.
#include "Utilities/Components/SemblanceComponent.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utilities/Components/WeaponComponent.h"
#include "Widgets/CharacterInformation.h"

// Sets default values for this component's properties
USemblanceComponent::USemblanceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	dashType = EDashType::Physical;
	dashCost = 10.0f;
	dashStrength = 1700.0f;
	dashDuration = 0.25f;
	dashCooldown = 0.75f;
}

// Called when the game starts
void USemblanceComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to APlayableCharacter in USemblanceComponent::BeginPlay!"));
		return;
	}

	owningCharacter->Tags.Add(FName("CanDash"));
}

// Called every frame
void USemblanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool USemblanceComponent::GetOwningCharacter()
{
	owningCharacter = Cast<APlayableCharacter>(GetOwner());
	if (!owningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to APlayableCharacter in USemblanceComponent::GetOwningCharacter!"));
		return false;
	}

	return true;
}

void USemblanceComponent::ResetDash()
{
	owningCharacter->Tags.Add(FName("CanDash"));
}

void USemblanceComponent::EndDash()
{
	owningCharacter->GetCharacterMovement()->GroundFriction = owningCharacter->groundFriction;
	owningCharacter->GetCharacterMovement()->BrakingDecelerationWalking = owningCharacter->walkingBrakingDeceleration;
	owningCharacter->GetCharacterMovement()->GravityScale = owningCharacter->gravityScale;
	owningCharacter->Tags.Remove(FName("IsDashing"));
	GetWorld()->GetTimerManager().SetTimer(dashTimer, this, &USemblanceComponent::ResetDash, dashCooldown, false);
}

void USemblanceComponent::StartDash()
{
	if (!GetOwningCharacter())
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to APlayableCharacter in USemblanceComponent::StartDash!"));
		return;
	}

	if (!owningCharacter->ActorHasTag(FName("CanDash")))
		return;

	owningCharacter->Tags.Remove(FName("CanDash"));
	owningCharacter->GetCharacterMovement()->GroundFriction = 0.0f;
	owningCharacter->GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;
	owningCharacter->GetCharacterMovement()->GravityScale = 0.0f;
	owningCharacter->GetCharacterMovement()->Velocity = FVector(0.0f, 0.0f, 0.0f);
	FVector dashDirection = owningCharacter->GetLastMovementInputVector();
	dashDirection.Normalize();
	if (dashDirection.Length() > 0.25f)
	{
		owningCharacter->LaunchCharacter(dashDirection * dashStrength, true, true);
	}
	else
	{
		dashDirection = owningCharacter->cameraComponent->GetForwardVector();
		dashDirection.Z = 0;
		owningCharacter->LaunchCharacter(dashDirection * dashStrength, true, true);
	}

	owningCharacter->Tags.Add(FName("IsDashing"));
	GetWorld()->GetTimerManager().SetTimer(dashTimer, this, &USemblanceComponent::EndDash, dashDuration, false);
}