// Fill out your copyright notice in the Description page of Project Settings.
#include "Animations/PlayableCharacterAnim.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayableCharacterAnim::NativeInitializeAnimation()
{
	owningCharacter = Cast<ACharacter>(GetOwningActor());
	if (!owningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to ACharacter in UPlayableCharacterAnim!"));
		return;
	}

	if (owningCharacter->GetCharacterMovement())
	{
		movementComponent = owningCharacter->GetCharacterMovement();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get UCharacterMovementComponent in UPlayableCharacterAnim!"));
		return;
	}
}

void UPlayableCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(movementComponent))
	{
		velocity = movementComponent->Velocity;
		groundSpeed = velocity.Length();
		isFalling = movementComponent->IsFalling();
		isAiming = owningCharacter->ActorHasTag(FName("IsAiming"));
		if (groundSpeed > 3.0f && movementComponent->GetCurrentAcceleration() != FVector::Zero())
			shouldMove = true;
		else
			shouldMove = false;
	}

	APawn* pawn = TryGetPawnOwner();
	if (pawn)
	{
		groundSpeed = pawn->GetVelocity().Size();
		direction = CalculateDirection(pawn->GetVelocity(), pawn->GetControlRotation());
	}
	else
		PreviewWindowUpdate();
}

void UPlayableCharacterAnim::PreviewWindowUpdate_Implementation()
{
	/*if (debugFire)
	{
		debugFire = false;
		FireAnimation();
	}*/
}