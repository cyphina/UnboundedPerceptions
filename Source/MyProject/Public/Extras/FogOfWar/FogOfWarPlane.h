// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogOfWarPlane.generated.h"

class ARTSGameState;
class ABasePlayer;
class UProceduralMeshComponent;

/**
 * @brief Creates visualizations of the visibility polygons for all allied actors.
 * First attempt at FOW based off https://www.redblobgames.com/articles/visibility/.
 * Is only a visual effect - the actual system which turns off interactivity for units outside range is in the vision system.
 */
UCLASS() class MYPROJECT_API AFogOfWarPlane : public AActor
{
   GENERATED_BODY()

   UPROPERTY(VisibleAnywhere)
   UProceduralMeshComponent* customMesh;

 public:
   AFogOfWarPlane();
   virtual void BeginPlay() override;

   TArray<ABasePlayer*> basePlayerRefs;

   UFUNCTION(BlueprintCallable)
   void UpdateVisionPlane();

 private:
   ARTSGameState* gameStateRef;
   TArray<int32>  triangles;
   TSet<FVector>  visionBlockerCorners;

   // Adds a triangle from 3 vertex ids
   void AddTriangle(const int32 v1, const int32 v2, const int32 v3);
};
