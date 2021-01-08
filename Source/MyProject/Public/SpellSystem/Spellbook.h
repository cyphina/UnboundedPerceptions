// Fill out your copyright notice in the Description page of Project Settings.

/**Spellbook class is just a skill tree for a hero*/

#pragma once
#include "MyProject.h"
#include "SpellBook.generated.h"

struct FGameplayAbilitySpec;
class UMySpell;
class ABaseHero;
class USpellBook;

DECLARE_EVENT_OneParam(USpellBook, FOnSpellLearned, TSubclassOf<UMySpell>);
DECLARE_EVENT_OneParam(USpellBook, FOnSpellUpgraded, TSubclassOf<UMySpell>);

/**
 * @brief Holds all the abilities a hero can learn
 */
UCLASS(Blueprintable)
class MYPROJECT_API USpellBook : public UObject
{
   GENERATED_BODY()

public:
   static USpellBook* CreateSpellBook(ABaseHero* heroRef);

   /** List of all the spells learnable in this spell book.  Edit this to add/remove skills*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spellbook")
   TArray<TSubclassOf<UMySpell>> availableSpells;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetLearnableSpells() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetLearnedSpells() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetUnknownSpells() const;

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   UMySpell* GetDefaultAbilityCopy(int spellIndex) const;
   
   UFUNCTION(BlueprintCallable, Category = "Spellbook")
   bool LearnSpell(int index);

   /** Update possible learnable spells */
   UFUNCTION(BlueprintCallable, Category = "Spellbook")
   void Update(int index);

   /**
    *Checks if a spell in this spell book is learned
    * @param spellBookIndex - Index of spell inside the spell book
    */
   UFUNCTION(BlueprintCallable, Category = "Spellbook")
   bool HasLearnedSpell(int spellBookIndex) const { return GetLearnedSpells().Contains(spellBookIndex); }

   FOnSpellLearned OnSpellLearned() { return OnSpellLearnedEvent; }
   FOnSpellUpgraded OnSpellUpgraded() { return OnSpellUpgradedEvent; }
   
   void Respec();
   bool LearnNewSpell(UMySpell* spellObject);
   bool TryUpgradeSpellLevel(UMySpell* spellObject, FGameplayAbilitySpec* abilityInfo);

   void UpgradeSpell();

private:
   USpellBook();

   struct SpellNode
   {
      int                   index; // Index within the spellbook
      TSubclassOf<UMySpell> spellRef;
      TArray<SpellNode>     prereqSpellNodes;
      TArray<SpellNode>     unlockedSpellNodes;

      SpellNode(int index, TArray<SpellNode> prevSpellNodes, TArray<SpellNode> nextSpellNodes, TSubclassOf<UMySpell> spellRef) :
         index(index), spellRef(spellRef), prereqSpellNodes(prevSpellNodes), unlockedSpellNodes(nextSpellNodes) { }
   };
   
   void Init();
   
   /** Checks to see if spell is in learnable spells list */
   bool IsLearnable(SpellNode sNode);

   bool CheckLevel(UMySpell* spellObject) const;
   bool CheckPoints() const;
   bool CheckPrereqs(UMySpell* spellObject) const;
   bool CheckIfSpellMaxLevel(FGameplayAbilitySpec* abilityInfo, UMySpell* spellObject) const;
   
   class AUserInput* cpcRef;
   ABaseHero* heroRef;
   
   TDoubleLinkedList<SpellNode> learnableSpells; // Spells we haven't learned but have the prerequisite skills to learn but may not have the levels
   TDoubleLinkedList<SpellNode> learnedSpells;   // Spells we have learned
   TDoubleLinkedList<SpellNode> unknownSpells;   // Spells we haven't learned
   TMap<int, SpellNode>         spellNodes;      // List of all spell nodes -- maps can have invalidated elements so watch out pointing to them.  Just copy values for now

   FOnSpellLearned OnSpellLearnedEvent;
   FOnSpellUpgraded OnSpellUpgradedEvent;
};
