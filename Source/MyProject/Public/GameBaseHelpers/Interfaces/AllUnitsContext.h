// Created 9/30/20 3:53 PM

#pragma once
#include "AllUnitsContext.generated.h"

class AEnemy;
class AAlly;
class AUnit;
UINTERFACE(MinimalAPI)
class UAllUnitsContext : public UInterface
{
   GENERATED_BODY()
};

class IAllUnitsContext
{
   GENERATED_BODY()

 public:
   /** Registers an ally in a data structure that can be replicated amongst players (unlike the one in BasePlayer) */
   FORCEINLINE virtual void RegisterFriendlyUnit(AAlly* friendlyUnit) = 0;
   /** Registers an enemy in a data structure that can be replicated amongst players (unlike the one in BasePlayer) */
   FORCEINLINE virtual void RegisterEnemyUnit(AEnemy* enemyUnit) = 0;
   /** Removes an ally in a data structure that can be replicated amongst players (unlike the one in BasePlayer) */
   FORCEINLINE virtual void UnRegisterFriendlyUnit(AAlly* friendlyUnit) = 0;
   /** Removes an enemy in a data structure that can be replicated amongst players (unlike the one in BasePlayer) */
   FORCEINLINE virtual void UnRegisterEnemyUnit(AEnemy* enemyUnit) = 0;

   FORCEINLINE virtual const TArray<AUnit*>& GetAllAllyUnits() const = 0;
   FORCEINLINE virtual const TArray<AUnit*>& GetAllEnemyUnits() const    = 0;
};
