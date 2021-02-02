// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AllUnitsContext.h"
#include "GameFramework/GameStateBase.h"
#include "UpGameClock.h"
#include "VisionContext.h"
#include "Interfaces/GameSpeedContext.h"
#include "RTSGameState.generated.h"

class URTSIngameWidget;
class AEnemy;
class AAlly;
class AUnit;
class AFogOfWarPlane;
class UVisionSubsystem;

/**
 * Game State has information that is replicated to all clients.
 * Put here things that all players need to know about how the current game is progression (as opposed to GameMode which has game information that should be exclusive to the server)
 * Keep track of the state of the game (connected players).  1 Game State has many Player States, while each player states reference the 1 Game State
 */
UCLASS()
class MYPROJECT_API ARTSGameState : public AGameStateBase, public IAllUnitsContext, public IGameSpeedContext, public IVisionContext
{
   GENERATED_BODY()

 public:
   ARTSGameState();

   UFUNCTION(BlueprintCallable)
   const TSet<AUnit*>& GetAllFriendlyUnits() const override { return allyList; }

   UFUNCTION(BlueprintCallable)
   const TSet<AUnit*>& GetAllEnemyUnits() const override { return enemyList; }

   UFUNCTION(BlueprintCallable)
   const TSet<AUnit*>& GetVisibleEnemies() const override;

   UFUNCTION(BlueprintCallable)
   const TSet<AUnit*>& GetVisiblePlayerUnits() const override;

   /** Callback to update our time unit when we change game speed (done only within GameSpeedWidget */
   UFUNCTION()
   void UpdateGameSpeed(float newSpeedMultiplier);

   const FUpGameClock* GetGameClock() const { return clock.Get(); }

   void AddGameTime(FUpTime timeToAdd, FUpDate daysToAdd);
   void SetGameTime(FUpTime timeToAdd, FUpDate daysToAdd);

   float GetGameSpeed() const override { return speedModifier; }

   FUpdateGameSpeed OnGameSpeedUpdated() const override { return UpdateGameSpeedDelegate; }

 protected:
   void Tick(float deltaSeconds) override;
   void BeginPlay() override;

 private:
   void OnAllyActiveChanged(AAlly* allyRef, bool isActive);
   void OnEnemyActiveChanged(AEnemy* enemyRef, bool isActive);

   /** Registers an ally in a data structure that can be replicated amongst players (unlike the one in BasePlayer) */
   void RegisterFriendlyUnit(AAlly* friendlyUnit) override;
   /** Registers an enemy in a data structure that can be replicated amongst players (unlike the one in BasePlayer) */
   void RegisterEnemyUnit(AEnemy* enemyUnit) override;
   /** Removes an ally in a data structure that can be replicated amongst players (unlike the one in BasePlayer) */
   void UnRegisterFriendlyUnit(AAlly* friendlyUnit) override;
   /** Removes an enemy in a data structure that can be replicated amongst players (unlike the one in BasePlayer) */
   void UnRegisterEnemyUnit(AEnemy* enemyUnit) override;

   /** As units get killed off due to level unload, we need to make sure these data structures are on good shape when we resume playing in the next level */
   UFUNCTION()
   void CleanupUnitLists();

   UPROPERTY(EditDefaultsOnly, Category = "Vision", Meta = (AllowPrivateAccess = true))
   TSubclassOf<AFogOfWarPlane> FOWplaneClass;

   /** How many seconds in game does one second IRL correspond to when at 1x speed? */
   UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess))
   float defaultGameTimeSpeed = 60;

   UPROPERTY(BlueprintAssignable, Meta = (AllowPrivateAccess = true))
   mutable FUpdateGameSpeed UpdateGameSpeedDelegate;

   TUniquePtr<FUpGameClock> clock;

   float speedModifier = 1;

   UPROPERTY()
   UVisionSubsystem* visionManager;

   /**
    * Lists all party members that exist between every player (including other teammate players in co op)
    * Faster to keep stuff in a set for quick removal (granted items are kept together so we don't
    * suffer in terms of cache coherency.
    */
   TSet<AUnit*> allyList;

   /** Lists of all enemies in the level */
   TSet<AUnit*> enemyList;
};
