// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/Projectile.h"
#include "Actors/Characters/PlayableCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utilities/Components/HealthComponent.h"

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	sphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SetRootComponent(sphereCollision);

	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("SphereMesh");
	sphereMesh->SetCollisionProfileName("NoCollision");

	projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	projectileMovementComponent->ProjectileGravityScale = 0;

	sphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::BindToHurt);

	sphereMesh->SetRelativeScale3D(FVector(0.08f));
	sphereCollision->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	sphereMesh->SetupAttachment(sphereCollision);

	lifetime = 5.f;
	velocity = 20.f;
	damage = 5.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	projectileMovementComponent->InitialSpeed = velocity;
	projectileMovementComponent->MaxSpeed = velocity;
	AController* controller = Cast<AController>(GetOwner());

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &AProjectile::K2_DestroyActor, lifetime);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::BindToHurt(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<AProjectile>(OtherActor))
	{
		APlayableCharacter* character = Cast<APlayableCharacter>(OtherActor);
		if (character)
		{
			//UGameplayStatics::ApplyDamage(OtherActor, damage, nullptr, this, UDamageType::StaticClass());
			character->healthComponent->TakeDamage(damage);
		}

		Destroy();
	}
}

/*void AProjectile::BindToOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}*/