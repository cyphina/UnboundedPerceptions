// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WorldObjects/Unit.h"
#include "Stats/UnitStatStruct.h"
#include "Items/Item.h"
#include "Enemy.generated.h"

/**
 * Base class for all enemy units
 */

class AUserInput;
class ARTSGameMode;
class ARTSGameState;

USTRUCT(BlueprintType, NoExport)
struct FSpellCombination {
   TArray<TSubclassOf<UMySpell*>> combination;            // spells to be used in tandem
   TArray<int>                    delay;                  // delay between two actions
   TArray<int>                    vulnerabilityThreshold; // continue with action if target passes this threshold
};

USTRUCT(BlueprintType, NoExport)
struct FItemDrop {
   FMyItem itemInfo;
   int     dropPerc; //out of 100%
};

UCLASS()
class MYPROJECT_API AEnemy : public AUnit
{
   GENERATED_BODY()

   /** Range enemy will attack you */
   int aggroRange;

   /** If this enemy is in a combat ready state */
   bool isActive;

 public:
   AEnemy(const FObjectInitializer& oI);

   /** Lets us set initial stat values for our enemy */
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Parameters")
   FDefaultStats initialStats;

   /**How much money given on death*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Parameters")
   int expGiven;

   /**How much exp given on death*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Parameters")
   int moneyGiven;

   /**List of items that can be dropped on death*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Parameters")
   TArray<FItemDrop> itemDrops;

   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
   TArray<FSpellCombination> combinations; // List of combinations by priority.  Combination 0 will always be an opener

   /**What enemies are in our radius determined via sphere overlap events*/
   UPROPERTY()
   TSet<AUnit*> possibleEnemiesInRadius;

   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void Die_Implementation() override;
   void EndPlay(EEndPlayReason::Type e) override; /**Called only when deleted during play*/
   void Destroyed() override;                     /**Can be called when deleted in editor*/
   void Attack_Implementation() override;
   bool CastSpell(TSubclassOf<UMySpell> spellToCast) override;

   void SetSelected(bool value) override final;
   void SetEnabled(bool bEnabled) override;

   /**Sets a target as active/inactive, which tells the game that this enemy's ai is active*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
   bool GetIsActive() const { return isActive; }

   UFUNCTION(BlueprintCallable, Category = "Combat")
   void SetIsActive(bool value) { isActive = value; }

   TSet<AUnit*>* GetSeenEnemies() override;

 private:

   bool IsVisible() override;

   UFUNCTION()
   void OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep,
                              const FHitResult& sweepRes);

   UFUNCTION()
   void OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

   void InitializeStats();

   float CalculateTargetRisk() override;
};
