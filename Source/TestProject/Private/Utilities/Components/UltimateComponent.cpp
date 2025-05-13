// Fill out your copyright notice in the Description page of Project Settings.
#include "Utilities/Components/UltimateComponent.h"
#include "Actors/Characters/PlayableCharacter.h"

// Sets default values for this component's properties
UUltimateComponent::UUltimateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// Ultimate
	maxUltimate = 100.0f;
	ultimate = maxUltimate;
}

// Called when the game starts
void UUltimateComponent::BeginPlay()
{
	Super::BeginPlay();
	owningCharacter = Cast<APlayableCharacter>(GetOwner());
	if (!owningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast to APlayableCharacter in UUltimateComponent::BeginPlay!"));
		return;
	}
}

// Called every frame
void UUltimateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}