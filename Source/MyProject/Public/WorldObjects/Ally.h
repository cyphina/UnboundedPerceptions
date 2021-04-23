#pragma once

#include "Unit.h"
#include "Ally.generated.h"

class AUserInput;
class AAllyAIController;
class AEnemy;
class UPatrolComponent;
class URTSStateComponent;

UENUM(BlueprintType)
enum class EAllyBehavioralMode : uint8
{
   ABM_Passive,   // Heal and buff allies but run away of enemies are nearby
   ABM_Defensive, // Avoid enemies, focus on healing and attack if the enemy isn't attacking back
   ABM_Neutral,   // Player controlled behavior (may change this to doing both controlled attack and defensive options)
   ABM_Offensive, // Attack closest enemy and try to use spells as efficiently as possible
   ABM_Aggressive // Attack lowest hp enemy and drop all spells on weak enemy
};

/*
 * Class for all controllable allied units.  Can't derive from this in Blueprints, can only derive from Summons or BaseHero
 */
UCLASS(Abstract, HideCategories = (Physics, Tags, Cooking, Clothing), NotBlueprintable)
class MYPROJECT_API AAlly : public AUnit
{
   GENERATED_BODY()

   friend class AAllyAIController;
   friend void SetupAlliedUnits();

 public:
   AAlly(const FObjectInitializer& oI);

   /**
    * Check to see if things are above us so we know to make the roofs transparent as we walk underneath them. Used in BPs (will try to move this to C++ soon)
    */
   UFUNCTION(BlueprintCallable, BlueprintCosmetic)
   bool GetOverlappingObjects(TArray<FHitResult>& hits);

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AI")
   AAllyAIController* GetAllyAIController() const { return allyController; }

   /** Players who are tired of microing can switch to one of the various AI modes which vary in their offensive and defensive play. */
   UFUNCTION(Server, Reliable, BlueprintCallable, Category = "AI Mode")
   void SwitchAIModes(EAllyBehavioralMode newMode);
	
   /** Polymorphic selection override for caching units in basePlayer */
   void SetUnitSelected(bool value) override;

   const TSet<AUnit*>& GetSeenEnemies() const;

   bool GetIsEnemy() const override final { return false; }

   void SetEnabled(bool bEnabled) override;

   EAllyBehavioralMode GetAllyBehaviorMode() const { return currentAllyBehavior; }

   UPROPERTY()
   AAllyAIController* allyController;

   static const FText STUNNED_TEXT;

 protected:
   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void EndPlay(const EEndPlayReason::Type eReason) override;
   void PossessedBy(AController* newAllyControllerRef) override;
   void Restart() override;

 private:
   /** What enemies are in our radius determined via sphere overlap events */
   UPROPERTY()
   TSet<AUnit*> possibleEnemiesInRadius;

   const TArray<AUnit*>* GetVisibleEnemies_Impl() const override;
   const TArray<AUnit*>* GetAllies_Impl() const override;
   const TArray<AUnit*>* GetEnemies_Impl() const override;

   EAllyBehavioralMode currentAllyBehavior;
};
