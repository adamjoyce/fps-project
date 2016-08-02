// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSProjectile.h"
#include "FPSGravityProjectile.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPSGravityProjectile : public AFPSProjectile
{
    GENERATED_BODY()

    //
    UPROPERTY(VisibleAnywhere, Category="Projectile")
    URadialForceComponent* RadialForceComp;

    // Sets default values for this actor's properties.
    AFPSGravityProjectile(const FObjectInitializer& ObjectInitializer);

    // Called when projectile hits something.
    UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
