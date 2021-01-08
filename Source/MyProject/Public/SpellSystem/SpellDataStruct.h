#pragma once

#include "GameplayTagContainer.h"
#include "SpellTargetingTypes.h"
#include "SpellDataStruct.generated.h"

/** This struct holds spell information */
USTRUCT(BlueprintType)
struct FSpellInfo {
   GENERATED_USTRUCT_BODY()

   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   FText name = FText();

   // spell required level
   UPROPERTY(BlueprintReadOnly, Category = "Spellbook")
   TArray<int> reqLevel;

   // max level of ability
   UPROPERTY(BlueprintReadOnly, Category = "Spellbook")
   int maxLevel;

   // Cooldown duration
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   TArray<float> cdDuration;

   // cast time
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   float castTime = 0;

   // extra time parameter for various usage
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   float secondaryTime = 0;

   // Mana cost
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   TArray<int> cost;

   // spell description
   UPROPERTY(BlueprintReadOnly, Category = "Spellbook")
   FText desc = FText();

   // element
   UPROPERTY(BlueprintReadOnly, Category = "Spell", meta = (Categories = "Combat.Element"))
   FGameplayTag elem;

   // Targeting rules. Pointer to const but the pointed to targeting may change since maybe spells change targeting like in Dota's talents/Aghs.
   const FUpSpellTargeting* targeting = nullptr;

   // spell range
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   TArray<int> range;

   // id of required spells needed to have learned before learning this one
   UPROPERTY(BlueprintReadWrite, Category = "Spellbook")
   TArray<int> preReqs;

   // duration effect lasts for if there is one
   UPROPERTY(BlueprintReadWrite, Category = "Spell")
   TArray<float> duration;

   // damage scaling of this spell.  Comes in array of 4-tuples (str,int,agl,und)
   UPROPERTY(BlueprintReadWrite, Category = "Spell")
   TArray<int> damage;

   // period of this spell if it has a periodic effect
   UPROPERTY(BlueprintReadWrite, Category = "Spell")
   TArray<float> period;

   UPROPERTY(BlueprintReadWrite, Category = "Spell")
   TArray<int> AOE;

   FSpellInfo() = default;
};