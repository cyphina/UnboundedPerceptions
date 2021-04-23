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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllySelected, AAlly*, selectedAlly);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllyDeselected, AAlly*, deselectedAlly);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllySelectionToggled, AAlly*, toggledAlly);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseHeroSelected, ABaseHero*, selectedBaseHero);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseHeroDeselected, ABaseHero*, deselectedBaseHero);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGroundSelected); // SelectGround,

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitDeselected);

DECLARE_DELEGATE_OneParam(FOnEnemySelectedWithoutDebugging, AUnit*);

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

   /** When an enemy is selected and we don't have the debug option to control enemy units on */
   FOnEnemySelectedWithoutDebugging& OnEnemySelectedWithoutDebugging() const { return OnEnemySelectedWithoutDebuggingEvent; }

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnAllySelected OnAllySelectedDelegate;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnAllyDeselected OnAllyDeselectedDelegate;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnAllySelectionToggled OnAllySelectionToggled;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnBaseHeroSelected OnHeroSelectedDelegate;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnBaseHeroDeselected OnHeroDeselectedDelegate;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnUnitSelected OnUnitSelectedDelegate;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnUnitDeselected OnUnitDeselectedDelegate;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnGroundSelected OnSelectionClearedDelegate;

 protected:
   void Initialize(FSubsystemCollectionBase& Collection) override;
   void Deinitialize() override;

 private:
   mutable FOnAllyActiveChanged             OnAllyActiveChangedEvent;
   mutable FOnHeroActiveChanged             OnHeroActiveChangedEvent;
   mutable FOnEnemyActiveChanged            OnEnemyActiveChangedEvent;
   mutable FOnSummonActiveChanged           OnSummonActiveChangedEvent;
   mutable FOnFocusedUnitChanged            OnFocusedUnitChangedEvent;
   mutable FOnHeroLevelUp                   OnHeroLevelUpEvent;
   mutable FOnEnemySelectedWithoutDebugging OnEnemySelectedWithoutDebuggingEvent;
};
