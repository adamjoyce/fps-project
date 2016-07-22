// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSCharacter.h"
#include "FPSProjectile.h"


AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Create the FPS camera component.
    FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("First person camera"));

    // Position the camera just above the character's eyeline.
    FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);

    // Allow the pawn to control roation.
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Pair the camera with the FPSCharacter.
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());

    // Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn).
    FirstPersonMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FirstPersonMesh"));
    FirstPersonMesh->SetOnlyOwnerSee(true);
    FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
    FirstPersonMesh->bCastDynamicShadow = false;
    FirstPersonMesh->CastShadow = false;

    // Everyone but the owner can see the third person mesh.
    GetMesh()->SetOwnerNoSee(true);
}

void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("We are using a FPSCharacter"));
    }
}

void AFPSCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

    // Setup gameplay key bindings.
    InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
    InputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
    InputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
    InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
    InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);
    InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);
}

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

void AFPSCharacter::OnStartJump()
{
    bPressedJump = true;
}

void AFPSCharacter::OnStopJump()
{
    bPressedJump = false;
}

void AFPSCharacter::OnFire()
{
    // Try and fire a projectile.
    if (ProjectileClass != NULL)
    {
        // Get camera tranform.
        FVector CameraLoc;
        FRotator CameraRot;
        GetActorEyesViewPoint(CameraLoc, CameraRot);

        // MuzzleOffset is in camera space, so transform it to world space before offsetting from the camera to find the final muzzle position.
        FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
        FRotator MuzzleRotation = CameraRot;
        // Skew the aim upwards.
        MuzzleRotation.Pitch += 5.75f;

        UWorld* const World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = Instigator;

            // Spawn the projectile at the muzzle.
            AFPSProjectile* const Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
            if (Projectile)
            {
                // Find launch direction.
                FVector const LaunchDir = MuzzleRotation.Vector();
                Projectile->InitVelocity(LaunchDir);
            }
        }
    }
}

