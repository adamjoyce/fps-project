// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSCharacter.h"
#include "FPSProjectile.h"


AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // How far the Hitscan ray will travel.
    MaxRaycastDistance = 10000.0f;

    // The force applied to an object when hit with the hitscan weapon.
    HitscanImpulse = 300000.0f;

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

    // Player movement.
    InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
    InputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
    InputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
    InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
    InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);

    // Weapon actions.
    InputComponent->BindAction("FireProjectile", IE_Pressed, this, &AFPSCharacter::OnProjectileFire);
    InputComponent->BindAction("FireHitscan", IE_Pressed, this, &AFPSCharacter::OnHitscanFire);
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

void AFPSCharacter::OnProjectileFire()
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
        MuzzleRotation.Pitch += 6.0f;

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

void AFPSCharacter::OnHitscanFire()
{
    // Get the camera point of view.
    FVector CameraLoc;
    FRotator CameraRot;
    Controller->GetPlayerViewPoint(CameraLoc, CameraRot);

    // Trace variables and parameters.
    const FVector TraceStart = CameraLoc;
    const FVector Direction = CameraRot.Vector();
    const FVector TraceEnd = TraceStart + (Direction * MaxRaycastDistance);
    FCollisionQueryParams TraceParams(FName(TEXT("TraceHitScanFire")), true, this);
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = false;
    TraceParams.bTraceComplex = true;

    FHitResult Hit(ForceInit);
    UWorld* const World = GetWorld();
    if (World)
    {
        World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
        DrawDebugLine(World, TraceStart, TraceEnd, FColor::Red, false, 1.0f);

        // Ignore static actors such as the floor.
        if (Hit.GetActor() != NULL && Hit.GetActor()->ActorHasTag("NoImpulse") == false)
        {
            Hit.GetComponent()->AddImpulseAtLocation(Direction * HitscanImpulse, Hit.ImpactPoint);
        }
    }
}

