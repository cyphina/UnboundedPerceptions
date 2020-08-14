// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FogOfWarPlane.generated.h"

class ARTSGameState;
class ABasePlayer;
class UProceduralMeshComponent;

/** Creates visualizations of the visibility polygons for all allied actors*/

UCLASS()
class MYPROJECT_API AFogOfWarPlane : public AActor
{
   GENERATED_BODY()

   UPROPERTY(VisibleAnywhere)
   UProceduralMeshComponent* customMesh;

 public:
   AFogOfWarPlane();
   virtual void BeginPlay() override;
   virtual void Tick(float DeltaTime) override;

   TArray<ABasePlayer*> basePlayerRefs;

   UFUNCTION(BlueprintCallable)
   void UpdateVisionPlane();

 private:
   ARTSGameState* gameStateRef;
   TArray<int32>  triangles;

   // Adds a triangle from 3 vertex ids
   void AddTriangle(int32 v1, int32 v2, int32 v3);
};
