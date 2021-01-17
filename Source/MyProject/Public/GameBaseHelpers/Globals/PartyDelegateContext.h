#pragma once
#include "LocalPlayerSubsystem.h"
#include "PartyDelegateContext.generated.h"

class AUnit;
class AEnemy;
class AAlly;
class ABaseHero;
class ASummon;
class ARTSPawn;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAllyActiveChanged, AAlly*, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHeroActiveChanged, ABaseHero*, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEnemyActiveChanged, AEnemy*, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSummonActiveChanged, ASummon*, bool);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnFocusedUnitChanged, AUnit*);
DECLARE_EVENT_OneParam(ABaseHero, FOnHeroLevelUp, ABaseHero*);
DECLARE_EVENT_OneParam(ARTSPawn, FOnUnitSlotSelected, AUnit*);

UCLASS()
class UPartyDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()

public:
   FOnAllyActiveChanged& OnAllyActiveChanged() const { return OnAllyActiveChangedEvent; }

   FOnHeroActiveChanged& OnHeroActiveChanged() const { return OnHeroActiveChangedEvent; }

   FOnEnemyActiveChanged& OnEnemyActiveChanged() const { return OnEnemyActiveChangedEvent; }

   FOnSummonActiveChanged& OnSummonActiveChanged() const { return OnSummonActiveChangedEvent; }

   FOnFocusedUnitChanged& OnFocusedUnitChanged() const { return OnFocusedUnitChangedEvent; }
   
   FOnHeroLevelUp& OnHeroLevelUp() const { return OnHeroLevelUpEvent; }

   FOnUnitSlotSelected& OnUnitSlotSelected() const { return OnUnitSlotSelectedEvent; }

protected:
   void Initialize(FSubsystemCollectionBase& Collection) override;
   void Deinitialize() override;

private:
   mutable FOnAllyActiveChanged   OnAllyActiveChangedEvent;
   mutable FOnHeroActiveChanged   OnHeroActiveChangedEvent;
   mutable FOnEnemyActiveChanged  OnEnemyActiveChangedEvent;
   mutable FOnSummonActiveChanged OnSummonActiveChangedEvent;
   mutable FOnFocusedUnitChanged  OnFocusedUnitChangedEvent;
   mutable FOnHeroLevelUp         OnHeroLevelUpEvent;
   mutable FOnUnitSlotSelected OnUnitSlotSelectedEvent;
};
