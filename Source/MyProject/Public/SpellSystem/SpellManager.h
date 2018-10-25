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
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   TArray<int> reqLevel;

   // max level of ability
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   int maxLevel;

   // Cooldown duration
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   TArray<float> cdDuration;

   // cast time
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   float casttime = 0;

   // Mana cost
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   TArray<int> cost;

   // spell description
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   FText desc = FText();

   // element
   UPROPERTY(BlueprintReadOnly, Category = "Spell", meta = (Categories = "Combat.Element"))
   FGameplayTag elem;

   // targetting rules
   UPROPERTY(BlueprintReadOnly, Category = "Spell", meta = (Categories = "Skill.Targetting"))
   FGameplayTag targetting = UGameplayTagsManager::Get().RequestGameplayTag("Skill.Targetting.Enemy");

   // spell range
   UPROPERTY(BlueprintReadOnly, Category = "Spell")
   TArray<int> range;

   // id of required spells needed to have learned before learning this one
   UPROPERTY(BlueprintReadWrite, Category = "Spell")
   TArray<int> preReqs;

   // duration effect lasts for if there is one
   UPROPERTY(BlueprintReadWrite, Category = "Spellbook")
   TArray<float> duration;

   // damage scaling of this spell.  Comes in array of 4-tuples (str,int,agl,und)
   UPROPERTY(BlueprintReadWrite, Category = "Spellbook")
   TArray<int> damage;

   // period of this spell if it has a periodic effect
   UPROPERTY(BlueprintReadWrite, Category = "Spellbook")
   TArray<float> period;

   UPROPERTY(BlueprintReadWrite, Category = "Spellbook")
   TArray<int> AOE;

   FSpellInfo() = default;
};

// Struct for parsing through data driven spelltable CSV file
USTRUCT(Blueprintable)
struct FSpellbookLookupRow : public FTableRowBase {
   GENERATED_USTRUCT_BODY()

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   FText Name = FText();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<int> LevelReq = TArray<int>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   int MaxLevel = 0;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<float> Cooldown = TArray<float>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   float Casttime = 0;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<int> Cost = TArray<int>();

   // here we interpret the row as a string so we can use string manipulations then convert it to a text
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   FText Desc = FText();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   FGameplayTag Elem = FGameplayTag();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   FGameplayTag Targetting = FGameplayTag();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<int> Range = TArray<int>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<int> PreReqs = TArray<int>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<float> Duration = TArray<float>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<int> Damage = TArray<int>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<float> Period = TArray<float>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
   TArray<int> AOE = TArray<int>();

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spellbook")
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

   UDataTable*           spellLookupTable; // Data table with the spell information
   TMap<int, FSpellInfo> spells;           // Map of spells by their id so we can always reference a spell given its id

   static void InitializeManager(); // Initializes spellmanager if none exists
   void        SetupSpells();
};
