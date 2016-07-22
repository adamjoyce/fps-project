// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProject.h"
#include "FPSHUD.h"

AFPSHUD::AFPSHUD(const FObjectInitializer& ObjectInitiliazer) : Super(ObjectInitiliazer)
{
    // Set the crosshair texzture.
    static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/FirstPerson/Textures/FirstPersonCrosshair.FirstPersonCrosshair'"));
    CrosshairTex = CrosshairTexObj.Object;
}

void AFPSHUD::DrawHUD()
{
    Super::DrawHUD();

    // Draws a simple crosshair in the centre of the screen.
    // Find the center of the canvas.
    const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

    // Align the texture by offsetting the position by half the texture width.
    const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5f)), (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)));

    // Draw the crosshair.
    FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
    TileItem.BlendMode = SE_BLEND_Translucent;
    Canvas->DrawItem(TileItem);
}


