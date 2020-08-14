// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RTSAbilitySystemComponent.generated.h"

/**
 * Custom ability component with extra functionality
 * All units need a copy of this if we want to allow them to get status effects. That means every unit has this.
 */

UCLASS()
class MYPROJECT_API URTSAbilitySystemComponent : public UAbilitySystemComponent
{
   GENERATED_BODY()

   URTSAbilitySystemComponent();

 public:
   void BeginPlay() override;

   /**Overriden to allow us to add things like purging buffs*/
   FActiveGameplayEffectHandle ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& Spec, FPredictionKey PredictionKey) override;

   /** List of abilities that are in unit's skill slots */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
   TArray<TSubclassOf<class UMySpell>> abilities;

   /** Do we have the resources necessary to cast this spell and make sure we don't have any status effects preventing us*/
   bool CanCast(TSubclassOf<UMySpell> spellToCheck);

   /** Attempts to removes invisibility effect. Usually called when a unit performs an action when they are invisible, thus breaking the invisibility*/
   void TryRemoveInvisibility();

   /** Starts the damage process which involves:
    * 1. Creating a damage gameplay effect and then setting it up with the parameters we want
    * 2. Applying it to a gameplay ability component
    * 3. Executing URTSDamageCalculation which handles calculating the damage value under the hood
    *    i. This handles modifying our vitals and showcasing the damage numbers under the hood
    */
   void ApplyDamageToSelf();

   void ApplyDamageToTarget(URTSAbilitySystemComponent* targetComponent);

 private:
   class AUnit* unitOwnerRef;
};
