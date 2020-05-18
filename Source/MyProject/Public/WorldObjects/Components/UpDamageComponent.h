// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageStructs.h"
#include "UpDamageComponent.generated.h"

DECLARE_EVENT_OneParam(UUpDamageComponent, FOnDamageTaken, const FUpDamage&); // Called when the attack animation finally

class AUnit;
struct FGameplayTagContainer;

/**A component used by any actor which is an independent damage source in the world (units, traps, animations, events).
 *A nonindependent damage source would be something that deals damage because it was spawned by a spellcaster like a lava pool spell.
 *Only units can receive damage however, so if we want to make something like barricades we'd just have an enemy unit with a custom mesh that blocks
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUpDamageComponent : public UActorComponent
{
   GENERATED_BODY()

 public:
   UUpDamageComponent();

 protected:
   virtual void BeginPlay() override;

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

   /** Helper function to quickly calculate attack and defense bonuses due to affinity and resistance */
   void CalculatePiercing(AUnit* unit, FUpDamage& d, bool isAtt) const;

 public:

   /** Damage a target from a not unit source*/
   UFUNCTION(BlueprintCallable)
   void DamageTarget(UPARAM(ref) FUpDamage& d, FGameplayTagContainer effects) const;

   /** Damage a target from a unit source*/
   UFUNCTION(BlueprintCallable)
   void UnitDamageTarget(UPARAM(ref) FUpDamage& d, FGameplayTagContainer effects) const;

   FOnDamageTaken OnDamageTaken;
};
