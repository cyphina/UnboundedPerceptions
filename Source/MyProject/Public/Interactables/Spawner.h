// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "Spawner.generated.h"

/**
 * Spawns enemy units when walked into
 */
UCLASS()
class MYPROJECT_API ASpawner : public ATriggerBox
{
   GENERATED_BODY()
   bool triggered;

   /* List of character types to spawn at or near the Spawn Location */
   UPROPERTY(EditAnywhere, Category = "Spawner")
   TArray<TSubclassOf<class ACharacter>> CharactersToSpawn;

   /* Spawn location.  If not set, no enemies spawn. */
   UPROPERTY(EditAnywhere, Category = "Spawner")
   FVector SpawnLocation;

   /*Number of copies of each type of character to spawn*/
   UPROPERTY(EditAnywhere, Category = "Spawner")
   TArray<int32> Counts;

   /*This value indicates the max number of times the spawner will trigger enemies.  If zero, the default of 1 is assumed*/
   UPROPERTY(EditAnywhere, Category = "Spawner")
   int32 MaxTriggers;

   ASpawner();

   UFUNCTION()
   void OnBeginOverlap(class UPrimitiveComponent* OverlappingComp, AActor* otherActor, class UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& hitResult);

   UFUNCTION()
   void OnEndOverlap(class UPrimitiveComponent* overlappedComp, class AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);


   void OnConstruction(const FTransform& Transform) override;

   void BeginPlay();
};
