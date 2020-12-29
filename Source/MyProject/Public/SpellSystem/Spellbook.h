// Fill out your copyright notice in the Description page of Project Settings.

/**Spellbook class is just a skill tree for a hero*/

#pragma once
#include "MyProject.h"
#include "SpellBook.generated.h"

class UMySpell;
class ABaseHero;
class USpellBook;

DECLARE_EVENT_OneParam(USpellBook, FOnSpellLearned, TSubclassOf<UMySpell>);

/**
 * @brief Holds all the abilities a hero can learn
 */
UCLASS(Blueprintable) class MYPROJECT_API USpellBook : public UObject
{
   GENERATED_BODY()

   struct SpellNode {
      int                   index; // Index within the spellbook
      TSubclassOf<UMySpell> spellRef;
      TArray<SpellNode>     prereqSpellNodes;
      TArray<SpellNode>     unlockedSpellNodes;

      SpellNode(int index, TArray<SpellNode> prevSpellNodes, TArray<SpellNode> nextSpellNodes, TSubclassOf<UMySpell> spellRef) :
          index(index), spellRef(spellRef), prereqSpellNodes(prevSpellNodes), unlockedSpellNodes(nextSpellNodes)
      {
      }
   };

   // Checks to see if spell is in learnable spells list
   bool IsLearnable(SpellNode sNode);

   UPROPERTY()
   class AUserInput* cpcRef;

 public:
   static USpellBook* CreateSpellBook(ABaseHero* heroRef);

   /**List of all the spells learnable in this spellbook.  Edit this to add/remove skills*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spellbook")
   TArray<TSubclassOf<UMySpell>> availableSpells;

   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   ABaseHero* heroRef;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetLearnableSpells() const;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetLearnedSpells() const;
   
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetUnknownSpells() const;

   UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Spells")
   UMySpell* GetDefaultAbilityCopy(int spellIndex) const;

   // Call to initialize skilltree.  Make sure to call this after constructing a spellbook!
   UFUNCTION(BlueprintCallable, Category = "Spellbook")
   void Init();

   UFUNCTION(BlueprintCallable, Category = "Spellbook")
   bool LearnSpell(int index);

   // Update possible learnable spells
   UFUNCTION(BlueprintCallable, Category = "Spellbook")
   void Update(int index);

   /**Checks if a spell in this spellbook is learned
    * @param spellBookIndex - Indice of spell inside the spellbook
    */
   UFUNCTION(BlueprintCallable, Category = "Spellbook")
   bool HasLearnedSpell(int spellBookIndex) const { return GetLearnedSpells().Contains(spellBookIndex); }

   void Respec();

   void UpgradeSpell();

 private:
   USpellBook();

   TDoubleLinkedList<SpellNode> learnableSpells; // spells we haven't learned but have the prerequisite skills to learn but may not have the levels
   TDoubleLinkedList<SpellNode> learnedSpells;   // spells we have learned
   TDoubleLinkedList<SpellNode> unknownSpells;   // spells we haven't learned
   TMap<int, SpellNode>         spellNodes;      // list of all spellnodes -- maps can have invalidated elements so watch out pointing to them.  Just copy values for now

   FOnSpellLearned OnSpellLearnedEvent;
};
