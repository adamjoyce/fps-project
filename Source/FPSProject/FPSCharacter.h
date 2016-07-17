// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPSPROJECT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
    /** First person camera */
    UPROPERTY()
    UCameraComponent* FirstPersonCameraComponent;

	// Sets default values for this character's properties
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


protected:
    // Called to bind functionality to input
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
};
