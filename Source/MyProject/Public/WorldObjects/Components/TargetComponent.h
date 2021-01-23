// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayAbilityTargetTypes.h"
#include "TVariant.h"
#include "UnitController.h"
#include "UnitTargetData.h"
#include "Components/ActorComponent.h"
#include "TargetComponent.generated.h"

class AUnit;

/**
 * A component used to give actor's the ability to target other actors/units/locations
 * Used in units that are not completely autonomous and use only hitbox based attacks
 * Used in buildings that may not be considered units
 * Used in NPCs possibly so they can have a follow target
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UTargetComponent : public UActorComponent
{
   GENERATED_BODY()

 public:
   UTargetComponent();

   /** Get data on our spell target.  Is reset after the spell is successfully cast*/
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   FGameplayAbilityTargetDataHandle GetTargetData() const { return Visit(TargetDataVisitor(), targetData.target); }

   /**
    * Get data on targeted unit.  Used in single target spell casting and attacking.
    * Reset after spell casted and if we attack a new target or stop.
    * Faster to use this then cast GetTargetData() to some unit type
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   AUnit* GetTargetUnit() const { return targetData.target.Get<AUnit*>(); }

   /** Checks to see if the variant is currently representing a target unit*/
   bool IsTargetingUnit() const { return targetData.target.TryGet<AUnit*>() != nullptr; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   AActor* GetTargetActor() const { return targetData.target.Get<AActor*>(); }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   FVector GetTargetLocation() const { return targetData.target.Get<FVector>(); }

   /** Use this to get the location of a target despite whatever kind of object is stored in the target variant*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "CombatAccessors")
   FVector GetTargetLocationVisit() const { return Visit(TargetLocationVisitor(), targetData.target); }

   FORCEINLINE void SetTarget(AUnit* value) { targetData.target.Set<AUnit*>(value); }
   FORCEINLINE void SetTarget(AActor* value) { targetData.target.Set<AActor*>(value); }
   FORCEINLINE void SetTarget(FVector value) { targetData.target.Set<FVector>(value); }
   FORCEINLINE void SetTarget(FGameplayAbilityTargetDataHandle value) { targetData.target.Set<FGameplayAbilityTargetDataHandle>(value); }

   void ResetTarget() { targetData.ResetTarget(); }

   AActor* GetTargetActorOrUnit() const;

   /** Check to see if we're targeting ourselves with a spell*/
   bool IsTargetingSelf() const;

 protected:
   /**
    * Call this function over setting values manually (which will be made private one day) because it sets the targetActor and targetUnit, both which are used
    * when dealing with unit tasks.
    */
   UFUNCTION(BlueprintCallable, Category = "CombatAccessors")
   void SetTargetUnit(AUnit* value) { targetData.target.Set<AUnit*>(value); }

   /** Intended use for targeting an actor that is not a unit*/
   UFUNCTION(BlueprintCallable, Category = "CombatAccessors")
   void SetTargetActor(AActor* value) { targetData.target.Set<AActor*>(value); }

   /** Used to target an actor that is not a unit*/
   UFUNCTION(BlueprintCallable, Category = "CombatAccessors")
   void SetTargetLocation(FVector value) { targetData.target.Set<FVector>(value); }

   UFUNCTION(BlueprintCallable, Category = "CombatAccessors")
   void SetSpellTarget(FGameplayAbilityTargetDataHandle value) { targetData.target.Set<FGameplayAbilityTargetDataHandle>(value); }

 private:
   UnitTargetData targetData;

   /** Used to get the location of a target (depending on what type it is) */
   class TargetLocationVisitor
   {
    public:
      FVector operator()(FEmptyVariantState) const;
      FVector operator()(FVector v) const;
      FVector operator()(AActor* a) const;
      FVector operator()(AUnit* u) const;
      FVector operator()(FGameplayAbilityTargetDataHandle h) const;
   };

   /** Used to get  a FGameplayAbilityTargetDataHandle from the target union. This struct is used in abilities */
   class TargetDataVisitor
   {
    public:
      FGameplayAbilityTargetDataHandle operator()(FEmptyVariantState) const;
      FGameplayAbilityTargetDataHandle operator()(FVector v) const;
      FGameplayAbilityTargetDataHandle operator()(AActor* a) const;
      FGameplayAbilityTargetDataHandle operator()(AUnit* u) const;
      FGameplayAbilityTargetDataHandle operator()(FGameplayAbilityTargetDataHandle h) const;
   };

   class SetTargetVisitor
   {
    public:
      SetTargetVisitor(UTargetComponent* targetCompRef) : targetCompRef{targetCompRef} {}
      void operator()(FEmptyVariantState) const;
      void operator()(FVector v) const;
      void operator()(AActor* a) const;
      void operator()(AUnit* u) const;
      void operator()(FGameplayAbilityTargetDataHandle h) const;

    private:
      UTargetComponent* targetCompRef;
   };
};
