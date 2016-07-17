// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSCharacter.h"


// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We are using a FPSCharacter"));
    }
}

// Called every frame
void AFPSCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

    // Setup gameplay key bindings.
    InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
}

// Moves the character forwards or backwards.
void AFPSCharacter::MoveForward(float Value)
{
    if (Controller != NULL && Value != 0.0f)
    {
        // Find out which way is forward.
        FRotator Rotation = Controller->GetControlRotation();

        // Limit pitch when walking or falling.
        if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
        {
            Rotation.Pitch = 0.0f;
        }

        // Add movement along the forward direction.
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

// Moves the character left or right.
void AFPSCharacter::MoveRight(float Value)
{
    if (Controller != NULL && Value != 0.0f)
    {
        // Find out which way is right.
        FRotator Rotation = Controller->GetControlRotation();

        // Add movement along the right direction.
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

