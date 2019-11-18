// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "StatChangeEffect.h"
#include "DamageStructs.h"
#include "SpellManager.generated.h"

class UMySpell;
class UMyGameplayEffect;

// Class for holding information about all the spells

// This struct holds our spell information used in our code and BP
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
   float casttime = 0;

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

   // targetting rules
   UPROPERTY(BlueprintReadOnly, Category = "Spell", meta = (Categories = "Skill.Targetting"))
   FGameplayTag targetting = UGameplayTagsManager::Get().RequestGameplayTag("Skill.Targetting.Single.Enemy");

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

// Struct for parsing through data driven spelltable CSV file
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
   float Casttime = 0;

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
   FGameplayTag Targetting = FGameplayTag();

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

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc")
   TSubclassOf<UMySpell> spellClass;
};

// Singleton class for a spellmanager that lets us get our spelldata table whenever we need information about a spell (aka our SpellInfo)
UCLASS()
class MYPROJECT_API USpellManager : public UObject
{
   GENERATED_BODY()
 public:
   USpellManager();
   ~USpellManager() = default;

   FORCEINLINE static USpellManager& Get()
   {
      if (SingletonManager == nullptr) { InitializeManager(); }
      return *SingletonManager;
   }

   FORCEINLINE FSpellInfo* GetSpellInfo(int spellID)
   {
      checkf(spells.Contains(spellID), TEXT("SpellID %d missing from spellList"), spellID);
      return &spells[spellID];
   }

   FORCEINLINE TSubclassOf<UMySpell> GetSpellClass(int spellID) { return spellClasses[spellID]; }

   TMap<int, TSubclassOf<UMySpell>> spellClasses;

 private:
   static USpellManager* SingletonManager; // Our single spellmanager

   TMap<int, FSpellInfo> spells;           // Map of spells by their id so we can always reference a spell given its id

   static void InitializeManager(); // Initializes spellmanager if none exists
   void        SetupSpells(UDataTable* spellLookupTable);
};
