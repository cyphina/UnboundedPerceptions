#pragma once

#include "WorldObjects/Unit.h"
#include "Stats/UnitStatStruct.h"
#include "Items/Item.h"
#include "Enemy.generated.h"

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
   int     dropPerc; // clamped from (0-100)
};

/**
 * Base class for all enemy units
 */
UCLASS()
class MYPROJECT_API AEnemy : public AUnit
{
   GENERATED_BODY()

 public:
   AEnemy(const FObjectInitializer& oI);

   /**Sets a target as active/inactive, which tells the game that this enemy's ai is active*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Combat")
   bool GetIsActive() const { return isActive; }

   UFUNCTION(BlueprintCallable, Category = "Combat")
   void SetIsActive(bool value) { isActive = value; }

   bool GetIsEnemy() const override final { return true; }

   FDefaultStats& GetInitialStats() { return initialStats; }

   const TSet<AUnit*>* GetVisibleEnemies_Impl() const override;
   const TSet<AUnit*>* GetAllies_Impl() const override;

   void SetSelected(bool value) override final;

 protected:
   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void EndPlay(const EEndPlayReason::Type e) override; /**Called only when deleted during play*/
   void Destroyed() override;                           /**Can be called when deleted in editor*/

   void SetEnabled(bool bEnabled) override;

   /**List of items that can be dropped on death*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Parameters")
   TArray<FItemDrop> itemDrops;

   /**How much money given on death*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Parameters")
   int expGiven;

   /**How much exp given on death*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Parameters")
   int moneyGiven;

   /** Lets us set initial stat values for our enemy */
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Parameters")
   FDefaultStats initialStats;

   /** Range enemy will attack you */
   UPROPERTY(EditAnywhere)
   int aggroRange;

   /** If this enemy is in a combat ready state */
   UPROPERTY(EditAnywhere)
   bool isActive;

 private:
   void GiveRewardsOnDeath();
   void InitializeStats();

   void SpawnItemDrops();
};
