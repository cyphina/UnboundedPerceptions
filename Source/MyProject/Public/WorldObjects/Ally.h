#pragma once

#include "Unit.h"
#include "Ally.generated.h"

class AUserInput;
class AAllyAIController;
class AEnemy;
class UPatrolComponent;
class URTSStateComponent;

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

   /**Check to see if things are above us so we know to make the roofs transparent as we walk underneath them. Used in BPs (will try to move this to C++ soon) */
   UFUNCTION(BlueprintCallable)
   bool GetOverlappingObjects(TArray<FHitResult>& hits);

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "AI")
   AAllyAIController* GetAllyAIController() const { return allyController; }

   /** Polymorphic selection override for caching units in basePlayer */
   void SetUnitSelected(bool value) override;

   const TSet<AUnit*>& GetSeenEnemies() const;

   bool GetIsEnemy() const override final { return false; }

   void SetEnabled(bool bEnabled) override;

   UPROPERTY()
   AAllyAIController* allyController;

   static inline const FText STUNNED_TEXT = NSLOCTEXT("HelpMessages", "Stun", "Currently Stunned!");

 protected:
   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void EndPlay(const EEndPlayReason::Type eReason) override;
   void PossessedBy(AController* newAllyControllerRef) override;

 private:
   /** What enemies are in our radius determined via sphere overlap events */
   UPROPERTY()
   TSet<AUnit*> possibleEnemiesInRadius;

   const TArray<AUnit*>* GetVisibleEnemies_Impl() const override;
   const TArray<AUnit*>* GetAllies_Impl() const override;
   const TArray<AUnit*>* GetEnemies_Impl() const override;
};
