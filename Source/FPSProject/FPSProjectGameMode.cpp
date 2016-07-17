// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSProjectGameMode.h"

AFPSProjectGameMode::AFPSProjectGameMode(const FObjectInitializer& ObjectInitiliazer) : Super(ObjectInitiliazer)
{
    // Sets the default pawn class to our FPSCharacter blueprint.
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("Blueprint'/Game/Blueprints/BP_FPSCharacter.BP_FPSCharacter_C'"));
    if (PlayerPawnObject.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnObject.Class;
    }
}

void AFPSProjectGameMode::StartPlay()
{
    Super::StartPlay();
    StartMatch();
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("SUP WORLD"));
    }
}