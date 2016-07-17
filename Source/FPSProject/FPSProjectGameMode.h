// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "FPSProjectGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPSProjectGameMode : public AGameMode
{
    GENERATED_BODY()

    AFPSProjectGameMode(const FObjectInitializer& ObjectInitiliazer);

    virtual void StartPlay();
};
