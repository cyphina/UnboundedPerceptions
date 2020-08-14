// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "SpellSystem/DamageStructs.h"
#include "RTSDamageCalculation.generated.h"

class AUnit;

/**Damage calculation is used to calculate the damage.  Grabs information from target and receiving unit to figure out damage amounts, as well as
 * using information gotten from the effect itself.
 * Logic for damage calculation is encapsulated her since we don't want to expose it anywhere. The actual damage creation is done inside the URTSGameplayAbility component by adding the
 * damage effect to self which will then use this calculation.
 */
UCLASS()
class MYPROJECT_API URTSDamageCalculation : public UGameplayEffectExecutionCalculation
{
   GENERATED_UCLASS_BODY()

   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                               OUT FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
   void BroadcastDamageEvents(FUpDamage& d) const;

 private:
   /** Plug in damage to damage formula and modify the damage amount based on crit or piercing */
   void CalculateDamage(FUpDamage& d, FGameplayTagContainer& effects) const;

   // Damage modification based off effects, buffs, or weapon effects
   void ApplyEffects(FUpDamage& d, FGameplayTagContainer& effects) const;

   /** Damage modification based off defensive effects.  If USING MAGICAL ARMOR, attacker may also get some debuff.  Called after damage reduction calculated */
   void ReceiveEffects(FUpDamage& damage, FGameplayTagContainer& effects) const;

   /** Damage calculation on receiving end, and possibly dodge check since accuracy is completely calculated after we look at target's evasion
       AUnit declares this as a friend function so we can access the setter methods */
   void CalculateDamageReduction(FUpDamage& damage, FGameplayTagContainer& effects) const;

   UFUNCTION(BlueprintCallable)
   void DamageTarget(UPARAM(ref) FUpDamage& d, FGameplayTagContainer effects) const;

   /** Damage a target from a unit source*/
   UFUNCTION(BlueprintCallable)
   void UnitDamageTarget(UPARAM(ref) FUpDamage& d, FGameplayTagContainer effects) const;

   /** Create some text to display the damage dealt to this unit */
   void ShowDamageDealt(const FUpDamage& damage);

   /** Helper function to quickly calculate attack and defense bonuses due to affinity and resistance */
   static void CalculatePiercing(AUnit* unit, FUpDamage& d, bool isAtt);
   static void PrintDamageCalcsBeforeProcessing(FUpDamage& d, int damageRange);
   static void PrintPreDamageReductionValues(FUpDamage& d);
   static void PrintCritOccurence(FUpDamage& d);
   static void PrintFinalCalculatedDamage(FUpDamage& d);
};
