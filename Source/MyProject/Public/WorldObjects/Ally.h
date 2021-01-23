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
   void SetSelected(bool value) override;

   void QueueAction(TFunction<void()> actionToQueue); // Queues an action to our action queue

   /** Accessor to clear command queue. */
   void ClearCommandQueue() { commandQueue.Empty(); }

   const TSet<AUnit*>& GetSeenEnemies() const;

   bool GetIsEnemy() const override final { return false; }

   const TSet<AUnit*>* GetVisibleEnemies_Impl() const override;
   const TSet<AUnit*>* GetAllies_Impl() const override;

   void SetEnabled(bool bEnabled) override;

   UPROPERTY()
   AAllyAIController* allyController;

   TQueue<TFunction<void()>, EQueueMode::Spsc> commandQueue;

   static inline const FText STUNNED_TEXT      = NSLOCTEXT("HelpMessages", "Stun", "Currently Stunned!");
   static inline const FText FILLED_QUEUE_TEXT = NSLOCTEXT("HelpMessages", "Queue", "Command Queue Filled!");

 protected:
   void BeginPlay() override;
   void Tick(float deltaSeconds) override;
   void EndPlay(const EEndPlayReason::Type eReason) override;
   void PossessedBy(AController* newAllyControllerRef) override;

 private:
   /** What enemies are in our radius determined via sphere overlap events */
   UPROPERTY()
   TSet<AUnit*> possibleEnemiesInRadius;

   int queueCount = 0;
};
