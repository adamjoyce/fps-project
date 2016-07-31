// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPSPROJECT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    // First person camera.
    UPROPERTY()
    UCameraComponent* FirstPersonCameraComponent;

    // Pawn mesh: 1st person view (arms; seen only by self).
    UPROPERTY(VisibleDefaultsOnly, Category="Mesh")
    USkeletalMeshComponent* FirstPersonMesh;

    // Gun muzzle's offset from the camera location.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gameplay")
    FVector MuzzleOffset;

    // Projectile class to spawn.
    UPROPERTY(EditDefaultsOnly, Category="Projectile")
    TSubclassOf<class AFPSProjectile> ProjectileClass;

	// Sets default values for this character's properties.
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;
	
	// Called every frame.
	virtual void Tick( float DeltaSeconds ) override;

    // Handles firing the projectiles.
    UFUNCTION()
    void OnProjectileFire();

    // Handles firing hitscan projectiles.
    UFUNCTION()
    void OnHitscanFire();

protected:
    // Called to bind functionality to input.
    virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    // Handles forward and backwards movement.
    UFUNCTION()
    void MoveForward(float Val);

    // Handles strafing movement.
    UFUNCTION()
    void MoveRight(float Val);

    // Sets the jump flag when key is pressed.
    UFUNCTION()
    void OnStartJump();

    // Clears the jump flag when key is released.
    UFUNCTION()
    void OnStopJump();

private:
    UPROPERTY()
    float MaxRaycastDistance;

    UPROPERTY()
    float HitscanImpulse;
};
