// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayableCharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UPlayableCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Character")
	ACharacter* owningCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCharacterMovementComponent* movementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	FVector velocity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	float groundSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	float direction;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool shouldMove;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool isFalling;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool isAiming;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	FName slotName;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) //Call function in code, but override in blueprint
	void PreviewWindowUpdate(); //This is what you want to call, DO NOT CREATE A BODY FOR THIS FUNCTION

	virtual void PreviewWindowUpdate_Implementation(); //This is what you want to override
};