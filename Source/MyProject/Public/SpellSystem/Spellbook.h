// Fill out your copyright notice in the Description page of Project Settings.

/**Spellbook class is just a skill tree for a hero*/

#pragma once
#include "MyProject.h"
#include "SpellBook.generated.h"

class UMySpell;
class ABaseHero;

UCLASS(Blueprintable)
class MYPROJECT_API USpellBook : public UObject
{
   GENERATED_BODY()

   class SpellNode
   {
    public:
      int                   index;
      TSubclassOf<UMySpell> spellRef;
      TArray<SpellNode>     prevSpellNodes; // how many prereqs do we need
      TArray<SpellNode>     nextSpellNodes; // what skills next
      SpellNode(int index, TArray<SpellNode> prevSpellNodes, TArray<SpellNode> nextSpellNodes, TSubclassOf<UMySpell> spellRef) :
          index(index), spellRef(spellRef), prevSpellNodes(prevSpellNodes), nextSpellNodes(nextSpellNodes)
      {
      }
   };

   TDoubleLinkedList<SpellNode> learnableSpells; // spells we haven't learned but have the prerequisite skills to learn but may not have the levels
   TDoubleLinkedList<SpellNode> learnedSpells;   // spells we have learned
   TDoubleLinkedList<SpellNode> unknownSpells;   // spells we haven't learned
   TMap<int, SpellNode>         spellNodes;      // list of all spellnodes -- maps can have invalidated elements so watch out pointing to them.  Just copy values for now

   bool isLearnable(SpellNode sNode);

 public:
   /**List of all the spells learnable in this spellbook.  Edit this to add/remove skills*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spellbook")
   TArray<TSubclassOf<UMySpell>> availableSpells;

   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = true))
   ABaseHero* heroRef;

   USpellBook();
   ~USpellBook();

   void PostInitProperties() override;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetLearnableSpells() const
   {
      TArray<int> spellIndices = TArray<int>();
      for (SpellNode sNode : learnableSpells) {
         spellIndices.Add(sNode.index);
      }
      return spellIndices;
   }
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetLearnedSpells() const
   {
      TArray<int> spellIndices = TArray<int>();
      for (SpellNode sNode : learnedSpells) {
         spellIndices.Add(sNode.index);
      }
      return spellIndices;
   }
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetUnknownSpells() const
   {
      TArray<int> spellIndices = TArray<int>();
      for (SpellNode sNode : unknownSpells) {
         spellIndices.Add(sNode.index);
      }
      return spellIndices;
   }

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
};
