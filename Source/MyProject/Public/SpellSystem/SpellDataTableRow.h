#pragma once

#include "GameplayTagContainer.h"
#include "SpellDataTableRow.generated.h"

/** Each row of our spell table has information about the spell's stats which are listed here */
USTRUCT(Blueprintable)
struct FSpellbookLookupRow : public FTableRowBase {
   GENERATED_USTRUCT_BODY()

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   FText Name = FText();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<int> LevelReq = TArray<int>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   int MaxLevel = 0;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   TArray<float> Cooldown = TArray<float>();

   // How long it takes to cast the spell (IncantationState)
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   float CastTime = 0;

   /* Used for various other timings a spell may need.  Most commonly it is used for confirmation spells,
   which require the user to press a button in a certain amount of time */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   float SecondaryTime = 0;

   // Mana cost of the skill
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   TArray<int> Cost = TArray<int>();

   // Here we interpret the row as a string so we can use string manipulations then convert it to a text
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   FText Desc = FText();

   /* Determines elemental properties of the damage dealt, however the spell may have different elemental affects or may use additional elements
   in the actual implementation*/
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   FGameplayTag Elem = FGameplayTag();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   FGameplayTag Targeting = FGameplayTag();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   TArray<int> Range = TArray<int>();

   // IDs of the spells that must be learned before this spell can be learned
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   TArray<int> PreReqs = TArray<int>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   TArray<float> Duration = TArray<float>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   TArray<int> Damage = TArray<int>();

   // Some effects apply themselves at regular periods, which is described in this field
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   TArray<float> Period = TArray<float>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
   TArray<int> AOE = TArray<int>();
};