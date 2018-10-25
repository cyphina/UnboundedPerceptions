// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WorldObjects/Unit.h"
#include "Stats/UnitStatStruct.h"
#include "Enemy.generated.h"

/**
 * Base class for all enemy units
 */

class AUserInput;
class ARTSGameMode;
class ARTSGameState;
struct FMyItem;

USTRUCT(BlueprintType, NoExport)
struct FSpellCombination {
   TArray<TSubclassOf<UMySpell*>> combination;            // spells to be used in tandem
   TArray<int>                    delay;                  // delay between two actions
   TArray<int>                    vulnerabilityThreshold; // continue with action if target passes this threshold
};

UCLASS()
class MYPROJECT_API AEnemy : public AUnit
{
   GENERATED_BODY()
   /**
    *range enemy will attack you
    */
   int aggroRange;

   /**
    *if this enemy is in a combat ready state
    */
   bool isActive;

 public:
   AEnemy(const FObjectInitializer& oI);

   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   FUnitStatStruct initialStats;

   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int expGiven; // how much money given on death
   UPROPERTY(BlueprintReadWrite, EditAnywhere)
   int moneyGiven; // how much exp given on death
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
   TArray<FSpellCombination> combinations; // List of combinations by priority.  Combination 0 will always be an opener

   /**What enemies are in our radius determined via sphere overlap events*/
   TSet<AAlly*> possibleEnemiesInRadius;

   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void Die() override;

   void SetSelected(bool value) override;

   /**Sets a target as active/inactive, which tells the game that this enemy's ai is active*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
   bool GetIsActive() const { return isActive; }

   /**/
   UFUNCTION(BlueprintCallable, Category = "Combat")
   void SetIsActive(bool value) { isActive = value; }

 private:
   AUserInput*    controllerRef;
   ARTSGameMode*  gameModeRef;
   ARTSGameState* gameStateRef;

   UFUNCTION()
   void OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep, const FHitResult& sweepRes);

   UFUNCTION()
   void OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

   void InitializeStats();
};
