<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.

=======
>>>>>>> componentrefactor
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "UpResourceManager.h"
#include "RTSCheatManager.generated.h"

/**
 * Cheat manager class to hold console commands
 */

class ARTSGameState;
class ARTSGameMode;
class AUserInput;
class AUnit;
class ABaseHero;
class AEnemy;

UCLASS(Within = PlayerController)
class MYPROJECT_API URTSCheatManager : public UCheatManager
{
   GENERATED_BODY()

<<<<<<< HEAD
   AUserInput*    userInputRef;
   ARTSGameMode*  gameModeRef;
   ARTSGameState* gameStateRef;

 public:
   virtual void InitCheatManager() override;

=======
 public:
>>>>>>> componentrefactor
   /** Level up hero once
    * @param heroName - Name of the hero in the party
    */
   UFUNCTION(exec, Category = "Cheats")
<<<<<<< HEAD
   virtual void LevelUp(FString heroName);
=======
   virtual void Up_LevelUp(FString heroName);
>>>>>>> componentrefactor

   /** Levels up hero to a certain level
    * @param heroName - Name of the hero in the party
    * @param level - What level do we stop leveling up to.  If less than current level does nothing
    */
   UFUNCTION(exec, Category = "Cheats")
<<<<<<< HEAD
   virtual void LevelUpToLevel(FString heroName, int level);
=======
   virtual void Up_LevelUpToLevel(FString heroName, int level);

   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_BuffAllHeroesStats();

   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_BuffAllEnemyStats();
>>>>>>> componentrefactor

   /** Prevents a unit from dying/taking damage
    * @param objectID - ID Name of the hero in the party
    * @param toggleGodMode - If 0, turns off god mode, else turns it on
    */
   UFUNCTION(exec, Category = "Cheats")
<<<<<<< HEAD
   virtual void GodMode(FString objectID, int toggleGodMode);
=======
   virtual void Up_GodMode(FString objectID, int toggleGodMode);
>>>>>>> componentrefactor

   /** Adds a quest
    * @param questName - Name of the quest to be completed that maps to a tag in BPQuestMap
    */
   UFUNCTION(exec, Category = "Cheats")
<<<<<<< HEAD
   virtual void AddQuest(FString questName);
=======
   virtual void Up_AddQuest(FString questName);
>>>>>>> componentrefactor

   /** Completes a quest that is accepted
    * @param questName - Name of the quest to be completed
    * @param isSucessful - Complete the quest sucesssfully? or fail quest?
    */
   UFUNCTION(exec, Category = "Cheats")
<<<<<<< HEAD
   virtual void FinishQuest(FString questName, int isSucessful);

   /** Completes a quest that is accepted
    * @param heroName - Hero which this spell should be equipped to
    * @param spellID - ID of the spell which should be added
    * @param slot - What slot should this spell go in
    */
   UFUNCTION(exec, Category = "Cheats")
   virtual void EquipSpell(FString heroName, int spellID, int slot);

   /** Refreshes the spells of all units (puts them off CD) */
   UFUNCTION(exec, Category = "Cheats")
   virtual void RefreshSpells();
=======
   virtual void Up_FinishQuest(FString questName, int isSucessful);

   /**
    * Gives the unit that is selected a spell, granted it has a spell slot to hold that spell.
    * @param spellNameTag - Spell gameplay tag suffix (e.g. if the spell's nametag is "Skill.Name.Snipe" then input "Snipe")
    * @param slot - What slot should this spell go in
    */
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_EquipSpell(FString spellNameTag, int slot);

   /** Refreshes the spells of all units (puts them off CD) */
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_RefreshSpells();
>>>>>>> componentrefactor

   /** Refreshes the spells of a certain hero.  If no heroname is specified refreshes spells of all heroes in party
    * @param heroName - Hero who should have all their spell cooldowns reset.  If unspecified, resets cds of every unit
    */
   UFUNCTION(exec, Category = "Cheats")
<<<<<<< HEAD
   virtual void RefreshHeroSpells(FString heroName);

   /**Stops game time from continuing*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void PauseGameTimer();

   /**Set what time it is in game*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void SetGameTime(int seconds, int minutes, int hours);

   /**Set what day it is in game*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void SetGameDay(int day, int month, int year);
=======
   virtual void Up_RefreshHeroSpells(FString heroName);

   /**Stops game time from continuing*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_PauseGameTimer();

   /**Set what time it is in game*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_SetGameTime(int seconds, int minutes, int hours);

   /**Set what day it is in game*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_SetGameDay(int day, int month, int year);
>>>>>>> componentrefactor

   /**Change unit current hp
    * @param unitName - Unit whose hp should be modified
    * @param hpVal - New unit's hp value
    */
   UFUNCTION(exec, Category = "Cheats")
<<<<<<< HEAD
   virtual void SetUnitCurHP(FString unitName, int hpVal);

   /**Gives vision over all the enemy units*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void SeeAll();

   /**Learn all the possible dialog topics*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void LearnAllTopics();

   /**Learn all the possible dialog topics*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void SetChapterAndSection(int chapter, int section);

   /**Spawn enemy from ID list*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void SpawnEnemies(FName id, int level, int numberToSpawn, FVector spawnLocation);

   /**Allows us to toggle on categories for CSV profiling*/
   UFUNCTION(exec, Category = "Debugging")
   void EnableCSVCategories(FString csvCategories);
=======
   virtual void Up_SetUnitCurHP(FString unitName, int hpVal);

   /**Gives vision over all the enemy units*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_SeeAll();

   /**Learn all the possible dialog topics*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_LearnAllTopics();

   /**Learn all the possible dialog topics*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_SetChapterAndSection(int chapter, int section);

   /**Spawn enemy from ID list*/
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_SpawnEnemies(FName id, int level, int numberToSpawn, FVector spawnLocation);

   /** Stops enemies from being autonomous */
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_ToggleEnemyAI();

   /** Shows collision capsules around every unit. They are used for queries and distance (Like move overlap) */
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_ShowDebugCapsules(bool bShouldShow);

   /** Shows vision sphere's around every unit */
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_ShowVisionSpheres(bool bShouldShow);

   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_DrawSphere(FString unitName, float sphereRadius, float duration);

   /**
    * @brief Used to debug the damage indicator
    * @param unitName 
    * @param damageAmount 
    * @param bCrit 
    */
   UFUNCTION(exec, Category = "Cheats")
   virtual void Up_DamageUnit(FString unitName, int damageAmount = 1, bool bCrit = false);

   /**
    * Allows us to toggle on categories for CSV
    * Probably best to use Superluminal or FramePro
   */
   UFUNCTION(exec, Category = "Debugging")
   void Up_EnableCSVCategories(FString csvCategories);

 protected:
   void InitCheatManager() override;

 private:
   AUserInput*    userInputRef;
   ARTSGameMode*  gameModeRef;
   ARTSGameState* gameStateRef;
>>>>>>> componentrefactor
};
