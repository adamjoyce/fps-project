// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSProjectGameMode.h"
#include "FPSCharacter.h"

AFPSProjectGameMode::AFPSProjectGameMode(const FObjectInitializer& ObjectInitiliazer) : Super(ObjectInitiliazer)
{
    DefaultPawnClass = AFPSCharacter::StaticClass();
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