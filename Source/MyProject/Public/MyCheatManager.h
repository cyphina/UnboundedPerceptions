// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "ResourceManager.h"
#include "MyCheatManager.generated.h"

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
class MYPROJECT_API UMyCheatManager : public UCheatManager
{
	GENERATED_BODY()

	AUserInput*									userInputRef;
	ARTSGameMode*								gameModeRef;
	ARTSGameState*								gameStateRef;

	virtual void								InitCheatManager() override;

	/** Level up hero once 
	 * @param heroName - Name of the hero in the party
	 */
	UFUNCTION(exec, Category = "Cheats")
	virtual void										LevelUp(FString heroName);

	/** Levels up hero to a certain level 
	 * @param heroName - Name of the hero in the party
	 * @param level - What level do we stop leveling up to.  If less than current level does nothing
	 */
	UFUNCTION(exec, Category = "Cheats")
	virtual void										LevelUpToLevel(FString heroName, int level);
	
	/** Prevents a unit from dying/taking damage
	* @param objectID - ID Name of the hero in the party
	* @param toggleGodMode - If 0, turns off god mode, else turns it on
	*/
	UFUNCTION(exec, Category = "Cheats")
	virtual void										GodMode(FString objectID, int toggleGodMode);

	/** Completes a quest that is accepted
	* @param questName - Name of the quest to be completed
	* @param isSucessful - Complete the quest sucesssfully? or fail quest?
	*/
	UFUNCTION(exec, Category = "Cheats")
	virtual void										FinishQuest(FString questName, int isSucessful);

	/** Completes a quest that is accepted
	* @param heroName - Hero which this spell should be equipped to
	* @param spellID - ID of the spell which should be added
	* @param slot - What slot should this spell go in
	*/
	UFUNCTION(exec, Category = "Cheats")
	virtual void										EquipSpell(FString heroName, int spellID, int slot);

	/**Stops game time from continuing*/
	UFUNCTION(exec, Category = "Cheats")
	virtual void										PauseGameTimer();

	/**Set what time it is in game*/
	UFUNCTION(exec, Category = "Cheats")
	virtual void										SetGameTime(int seconds, int minutes, int hours);

	/**Set what day it is in game*/
	UFUNCTION(exec, Category = "Cheats")
	virtual void										SetGameDay(int day, int month, int year);

	/**Change unit current hp
	 * @param unitName - Unit whose hp should be modified
	 * @param hpVal - New unit's hp value
	 */
	UFUNCTION(exec, Category = "Cheats")
	virtual void										SetUnitCurHP(FString unitName, int hpVal);

	/**Gives vision over all the enemy units*/
	UFUNCTION(exec, Category = "Cheats")
	virtual void										SeeAll();



};
