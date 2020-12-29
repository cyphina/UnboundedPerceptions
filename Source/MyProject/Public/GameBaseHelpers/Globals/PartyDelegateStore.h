#pragma once
#include "Object.h"
#include "PartyDelegateStore.generated.h"

class AEnemy;
class AAlly;
class ABaseHero;
class ASummon;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAllyActiveChanged, AAlly*, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHeroActiveChanged, ABaseHero*, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEnemyActiveChanged, AEnemy*, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSummonActiveChanged, ASummon*, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocusedUnitChanged, AUnit*);

class UPartyDelegateStore : public ULocalPlayerSubsystem
{
 public:
   FOnAllyActiveChanged&   OnAllyActiveChanged() const { return OnAllyActiveChangedEvent; }
   FOnHeroActiveChanged&   OnHeroActiveChanged() const { return OnHeroActiveChangedEvent; }
   FOnEnemyActiveChanged&  OnEnemyActiveChanged() const { return OnEnemyActiveChangedEvent; }
   FOnSummonActiveChanged& OnSummonActiveChanged() const { return OnSummonActiveChangedEvent; }
   FOnFocusedUnitChanged&  OnFocusedUnitChanged() const { return OnFocusedUnitChangedEvent; }

 protected:
   void Initialize(FSubsystemCollectionBase& Collection) override;
   void Deinitialize() override;

 private:
   mutable FOnAllyActiveChanged   OnAllyActiveChangedEvent;
   mutable FOnHeroActiveChanged   OnHeroActiveChangedEvent;
   mutable FOnEnemyActiveChanged  OnEnemyActiveChangedEvent;
   mutable FOnSummonActiveChanged OnSummonActiveChangedEvent;
   mutable FOnFocusedUnitChanged  OnFocusedUnitChangedEvent;
};
