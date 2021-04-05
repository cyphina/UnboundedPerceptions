#pragma once
#include "MyProject.h"
#include "SpellBook.generated.h"

<<<<<<< HEAD
=======
struct FGameplayTag;
>>>>>>> componentrefactor
struct FGameplayAbilitySpec;
class UMySpell;
class ABaseHero;
class USpellBook;

<<<<<<< HEAD
DECLARE_EVENT_OneParam(USpellBook, FOnSpellLearned, TSubclassOf<UMySpell>);
DECLARE_EVENT_OneParam(USpellBook, FOnSpellUpgraded, TSubclassOf<UMySpell>);

/**
 * @brief Holds all the abilities a hero can learn
=======
/**
 * All abilities start at level 1!
 * Holds all the abilities a hero can learn and contains data about what is unlocked/unlockable. All abilities start at level 1 when they are first unlocked!
>>>>>>> componentrefactor
 */
UCLASS(Blueprintable)
class MYPROJECT_API USpellBook : public UObject
{
   GENERATED_BODY()

<<<<<<< HEAD
public:
   static USpellBook* CreateSpellBook(ABaseHero* heroRef);

   /** List of all the spells learnable in this spell book.  Edit this to add/remove skills*/
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Spellbook")
   TArray<TSubclassOf<UMySpell>> availableSpells;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetLearnableSpells() const;
=======
   struct SpellNode
   {
      int                   index;
      TSubclassOf<UMySpell> spellRef;
      TArray<SpellNode*>    prereqSpellNodes;
      TArray<SpellNode*>    unlockedSpellNodes;

      SpellNode(int index, TArray<SpellNode*> prevSpellNodes, TArray<SpellNode*> nextSpellNodes, TSubclassOf<UMySpell> spellRef) :
          index(index), spellRef(spellRef), prereqSpellNodes(prevSpellNodes), unlockedSpellNodes(nextSpellNodes)
      {
      }

      SpellNode(const SpellNode& otherSpellNode) = default;

      bool operator==(const SpellNode& otherSpellNode) const { return spellRef == otherSpellNode.spellRef; }
   };

 public:
   static USpellBook* CreateSpellBook(ABaseHero* heroRef, TSubclassOf<USpellBook> spellBookClass);

   /** Spells that we have the reqs to learn */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetLearnableSpellsIndices() const;
>>>>>>> componentrefactor

   /** Spells that we already have learned */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
<<<<<<< HEAD
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
=======
   TArray<int> GetLearnedSpellIndices() const;

   /** Spells we don't have the reqs to learn */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Spellbook")
   TArray<int> GetUnknownSpellIndices() const;

   UFUNCTION(BlueprintCallable, Category = "Spellbook")
   void OnSpellSlotSelected(int index);

   /**
    * Checks if a spell in this spell book is learned
    * @param spellBookIndex - Index of spell inside the spell book
    */
   UFUNCTION(BlueprintCallable, Category = "Spellbook")
   bool HasLearnedSpell(int spellBookIndex) const { return GetLearnedSpellIndices().Contains(spellBookIndex); }

   TSubclassOf<UMySpell> GetSpellFromIndex(int slotIndex) const;

   /** Functionality to reset all the skills we learned */
   void OnRespec();

   const TArray<TSubclassOf<UMySpell>>& GetAvailableSpells() const { return availableSpells; }

   bool IsSpellLearnable(TSubclassOf<UMySpell> spellClass);

 protected:
   /** List of all the spells learnable in this spell book.  Edit this to add/remove skills*/
   UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Spellbook")
   TArray<TSubclassOf<UMySpell>> availableSpells;

 private:
   USpellBook() = default;

   void Init();

   bool TryUpgradeSpellLevel(UMySpell* spellObject, FGameplayAbilitySpec* abilityInfo);

   bool LearnNewSpell(TSubclassOf<UMySpell> spellToLearn);

   void CacheInitialLearnableAndUnknownSpells(TSubclassOf<UMySpell> spell);

   void SetupInitialSpellNodeConnections();

   bool CheckLevel(UMySpell* spellObject) const;
   bool CheckPoints() const;
   bool CheckPrereqs(UMySpell* spellObject);
   bool CheckIfSpellMaxLevel(FGameplayAbilitySpec* abilityInfo, UMySpell* spellObject) const;

   class AUserInput* cpcRef;
   ABaseHero*        heroRef;

   TDoubleLinkedList<TSubclassOf<UMySpell>> learnableSpells; // Spells we haven't learned but have the prerequisite skills to learn but may not have the levels
   TDoubleLinkedList<TSubclassOf<UMySpell>> learnedSpells;   // Spells we have learned
   TDoubleLinkedList<TSubclassOf<UMySpell>> unknownSpells;   // Spells we haven't learned

   TMap<TSubclassOf<UMySpell>, SpellNode> spellNodes; // Allows us to find the corresponding spell node for a spell. Manages the lifetime of all the SpellNodes.
>>>>>>> componentrefactor
};
