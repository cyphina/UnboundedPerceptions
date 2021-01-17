#pragma once

#include "DamageStructs.h"
#include "GameplayTagContainer.h"
#include "SpellDefaults.generated.h"

USTRUCT(BlueprintType)
struct FSpellDefaults
{
   GENERATED_BODY()

public:
   /** Spell Icon */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   UTexture2D* image;

   /** Used for cooldown purposes. Every spell has its own unique name tag. */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   FGameplayTag nameTag;

   /** Used for UI purposes */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   FText Name = FText();

   /** Level required for each spell upgrade */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TArray<int> LevelReq = TArray<int>();

   /** Number of times we can upgrade this spell */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   int MaxLevel = 0;

   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TArray<float> Cooldown = TArray<float>();

   /** Time it takes to perform incantation (required of all spells before casting) */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   float CastTime = 0;

   /** Can be used for channeling */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   float SecondaryTime = 0;

   /** How much mana this spell costs */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TArray<int> Cost = TArray<int>();

   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   FText Desc = FText();

   /** A tag deriving from Combat.Element */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   FGameplayTag Elem = FGameplayTag();

   /** The strategy used this class to target something when casting */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TSubclassOf<UUpSpellTargeting> Targeting;

   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TArray<int> Range = TArray<int>();

   /** Spells we need to learn before learning this spell */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TArray<TSubclassOf<UMySpell>> PreReqs = TArray<TSubclassOf<UMySpell>>();

   /** How long the primary effect this spell inflicts lasts for. We may have secondary effects with custom times. */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TArray<float> Duration = TArray<float>();

   /** How much damage (sometimes this is used to represent healing as well) this spell does depends on these stat scaling values */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TArray<FDamageScalarStruct> Damage = TArray<FDamageScalarStruct>();

   /** If the primary effect caused by this can be applied periodically (e.g. poison), this value tells us when the next tick occurs */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TArray<float> Period = TArray<float>();

   /** How large of an area this spell effects */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default Spell Parameters")
   TArray<int> AOE = TArray<int>();

   /** Used to describe the effects of the spell to help AI guide decisions on how to cast it */
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spell")
   FGameplayTagContainer descriptionTags;
};