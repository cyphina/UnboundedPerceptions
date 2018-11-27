// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MinigameCanvas.h"
#include "Kismet/KismetRenderingLibrary.h"

AMinigameCanvas::AMinigameCanvas()
{
   PrimaryActorTick.bCanEverTick = true;

   canvasMesh                    = CreateDefaultSubobject<UStaticMeshComponent>(FName("CanvasMesh"));
   canvasMesh->SetupAttachment(RootComponent);
}

void AMinigameCanvas::BeginPlay()
{
   renderTarget = UKismetRenderingLibrary::CreateRenderTarget2D(GetWorld(), 1024, 1024);
   UKismetRenderingLibrary::ClearRenderTarget2D(GetWorld(), renderTarget);
   Super::BeginPlay();
}

void AMinigameCanvas::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}

bool AMinigameCanvas::CheckDrawing()
{
   // read in render target
   TArray<FColor>                colorData;
   FTextureRenderTargetResource* drawing = renderTarget->GameThread_GetRenderTargetResource();
   drawing->ReadPixels(colorData);

   // check out how much of the drawing framework remains
   int pixelCount = 0;
   for (FColor& color : colorData) {
      if (color.R + color.B + color.G < 10) ++pixelCount;
   }

   GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::FromInt(pixelCount));
   return pixelCount < pixelCountProximityAmt;
}
