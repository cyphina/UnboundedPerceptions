// Created 9/12/20 7:3 PM

#pragma once
#include "VisionSubsystem.generated.h"

class ARTSGameState;
class AUnit;
class URTSVisionComponent;

UCLASS(Within = RTSGameState)
class MYPROJECT_API UVisionSubsystem : public UObject, public FRunnable
{
   GENERATED_BODY()

 public:
   /** Creates a new UVisionSubsystem* and it is on the onus of the class in which the UVisionSubsystem is being created in to manage its memory (probably using UROPERTY())*/
   static UVisionSubsystem* Create(UObject* outer);

   UFUNCTION(BlueprintCallable, BlueprintPure)
   const TSet<AUnit*>& GetVisibleEnemies() const { return visibleEnemies; }

   UFUNCTION(BlueprintCallable, BlueprintPure)
   const TSet<AUnit*>& GetVisiblePlayerUnits() const { return visiblePlayerUnits; }

   void ToggleEnemyPerspective();

 protected:
   void BeginDestroy() override;

 private:
   UVisionSubsystem();

   uint32 Run() override;

   void Stop() override;

   void OnPreLoadMap(const FString& MapName);
   void OnPostLoadMap(UWorld* World);

   UFUNCTION()
   void OnLevelLoaded();

   UFUNCTION()
   void OnLevelAboutToUnload();

   FHitResult                  visionHitResult;
   FCollisionObjectQueryParams queryParamVision;

   /**Lists what enemies are visible so we don't have to keep doing line traces which is an expensive op*/
   TSet<AUnit*> visibleEnemies;

   /**Lists what allies are visible so we don't have to keep doing line traces which is an expensive op*/
   TSet<AUnit*> visiblePlayerUnits;

   void AddVisibleAlly(AUnit* newAlly);
   void AddVisibleEnemy(AUnit* newEnemy);

   TSet<const URTSVisionComponent*> GetFriendlyVisionComps() const;
   TSet<const URTSVisionComponent*> GetEnemyVisionComps() const;

   ARTSGameState* gameStateRef;

   /**
    * *** !!! ALso handles killing of units to prevent data races !!! ***
    * Visibility of enemies is like a state machine which has six states
    * 1. Enemy enters vision range and we can see it - Add to possible enemies in radius and add to visible units
    * 2. Enemy enters vision range but is behind a wall - Add to possible enemies in radius but not to visible units
    * 3. Enemy leaves vision range - Remove from possible enemies in radius and from visible units
    * 4. Enemy leaves vision range but was behind a wall so we never saw it - Remove from possible enemies in radius but not from visible units
    * 5. Enemy peaks a wall and is now in vision - Add to visible enemies but not to possible enemies in radius
    * 6. Enemy walks behind a wall and is not in vision - Remove from visible enemies but not from possible enemies in radius
    */
   UFUNCTION()
   void UpdateVisibleEnemies();

   /**For more info look at comments of UpdateVisibleEnemies.  Used to list all the player units (units player controls) that
    * are visible to according to enemies so that they can filter through the visible targets in their AI systems.
    */
   UFUNCTION()
   void UpdateVisiblePlayerUnits();

   /** Caches what units were visible last check (caches only units that we're going to hide) */
   void StoreUnitsToHideThatWereVisibleLastCall(TSet<AUnit*>& lastCallCache, TSet<AUnit*>& unitsToPossiblyHide);

   bool CheckUnitInVision(AUnit* unit, const URTSVisionComponent* visionComp, TSet<AUnit*>& visibleUnits);
   /** Trace to a target gets blocked by walls and fails if target is not visible */
   bool LineOfSightToNonInvisUnit(AUnit* unit, const URTSVisionComponent* visionComp);

   /** Unhides units that pass the visibility check. */
   void MakeUnitsInVisionVisible(TSet<AUnit*>& unitsEligibleForHidingVisibleLastCheck);

   /**
    * Enemies from the last visibility check that were visible that are no longer visible get hidden here.
    * Even if due to timing issues visibleEnemies doesn't have the right units inside it, it doesn't matter because the timing is so narrow and this will be rerun
    */
   void MakeUnitsOutOfVisionInvisible(TSet<AUnit*>& unitsEligibleForHidingVisibleLastCheck, TSet<AUnit*>& unitsDeemedVisibleAfterCheck) const;

   FRunnableThread* visionUpdateThread;

   FThreadSafeCounter stopTaskCounter;

   bool bShowEnemyPerspective = false;
};
