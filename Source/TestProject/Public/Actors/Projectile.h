// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TESTPROJECT_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* sphereCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* sphereMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* projectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float lifetime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damage;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintAuthorityOnly)
	void BindToHurt(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};