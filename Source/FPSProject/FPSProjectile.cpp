// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSProjectile.h"


AFPSProjectile::AFPSProjectile(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Use a sphere as a simple collision representation.
    CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
    CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
    CollisionComp->InitSphereRadius(15.0f);
    RootComponent = CollisionComp;

    // Use a ProjectileMovementComponent to govern this projectile's movement.
    ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
    ProjectileMovement->UpdatedComponent = CollisionComp;
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 0.3f;

    // Die after 3 secondsby default.
    InitialLifeSpan = 3.0f;
}

void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AFPSProjectile::InitVelocity(const FVector& ShootDirection)
{
    if (ProjectileMovement)
    {
        // Set the projectile's velocity to the desired direction.
        ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
    }
}

void AFPSProjectile::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && (OtherActor != this) && OtherComp && OtherActor->ActorHasTag("NoImpulse") == false)
    {
        OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
    }
}

