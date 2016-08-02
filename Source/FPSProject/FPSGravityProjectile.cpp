// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSGravityProjectile.h"

AFPSGravityProjectile::AFPSGravityProjectile(const FObjectInitializer& ObjectInitializer) : AFPSProjectile(ObjectInitializer) 
{
    RadialForceComp = ObjectInitializer.CreateDefaultSubobject<URadialForceComponent>(this, TEXT("RadialForceComp"));
    RadialForceComp->ForceStrength = -5000000.0f;
    RadialForceComp->Radius = 1000.0f;
}

void AFPSGravityProjectile::OnHit(UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        RadialForceComp->SetRelativeLocation(CollisionComp->RelativeLocation);
        RadialForceComp->SetActive(true);
    }
}


