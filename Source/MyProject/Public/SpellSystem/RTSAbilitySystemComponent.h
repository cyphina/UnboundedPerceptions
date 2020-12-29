// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RTSAbilitySystemComponent.generated.h"

class UMySpell;

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

   /** Get the class of the spell at this slot (CHECKED INDEX ACCESS) */
   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   TSubclassOf<UMySpell> GetSpellAtSlot(int index) const;

   void SetSpellAtSlot(TSubclassOf<UMySpell> spellClassToSet, int slotIndex);

   const TArray<TSubclassOf<class UMySpell>>& GetAbilities() const { return abilities; }

   int FindSlotIndexOfSpell(TSubclassOf<UMySpell> spellToLookFor) const;


   /** Do we have the resources necessary to cast this spell; also make sure we don't have any status effects preventing us*/
   bool CanCast(TSubclassOf<UMySpell> spellToCheck);

   /** Attempts to removes invisibility effect. Usually called when a unit performs an action when they are invisible, thus breaking the invisibility*/
   void TryRemoveInvisibility();

   /** Overriden to allow us to add things like purging buffs*/
   FActiveGameplayEffectHandle ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& Spec, FPredictionKey PredictionKey) override;

   void ApplyDamageToSelf();
   void ApplyDamageToTarget(URTSAbilitySystemComponent* targetComponent);

 protected:
   /** List of abilities that are in unit's skill slots */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
   TArray<TSubclassOf<class UMySpell>> abilities;

 private:
   class AUnit* unitOwnerRef;
};
