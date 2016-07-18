// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FPSProjectile.generated.h"

UCLASS()
class FPSPROJECT_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
    // Sphere collision component.
    UPROPERTY(VisibleDefaultsOnly, Category="Projectile")
    USphereComponent* CollisionComp;

    // Projectile movement component.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
    UProjectileMovementComponent* ProjectileMovement;

	// Sets default values for this actor's properties.
	AFPSProjectile(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;
	
	// Called every frame.
	virtual void Tick( float DeltaSeconds ) override;

    // Initialises velocity of the projectile in the shoot direction.
    void InitVelocity(const FVector& ShootDirection);
};
