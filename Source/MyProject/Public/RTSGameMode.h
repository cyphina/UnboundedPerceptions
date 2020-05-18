// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "SaveLoadClass.h"
#include "BasePlayer.h"
#include "RTSGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelLoaded);

/**
 * Game mode only exists on server.  Put things that only the server needs to know and use.  Can't replicate variables here, but calls here apply to server logic
 */

class UEventManager;
class UTriggerManager;
class UQuestManager;
class ULoadingWidget;
class UConditionalManager;
class UMinigameManager;
class IWorldObject;

UCLASS()
class MYPROJECT_API ARTSGameMode : public AGameModeBase
{
   GENERATED_BODY()

   ///< summary> Level Names </summary>

   const FString startingLevelName = "StartMap";
   const FString sylphiaApartment  = "SylphiaApartment";
   const FString roadToWubville    = "RoadToWubville";
   const FString blockadeCity      = "BlockadeCity";
   const FString factory           = "Factory";

   /**Stores the currently loaded level name*/
   UPROPERTY(BlueprintReadWrite, Category = "Levels", Meta = (AllowPrivateAccess = "true"))
   FString currentLevelName;

   /**Did we just load a new level because we loaded a game load?*/
   bool bLoading;

   void BeginPlay() override;

   // Injects this dependency in the classes it spawnss
   UPROPERTY()
   class AHUDManager* hudManagerRef;

 public:
   ARTSGameMode();

   ///---Expose these classes so we can spawn a more derived blueprint class version of each manager in the code---

   /**
    * EventManager - Handles progressing in story and activating story based triggers
    */
   UPROPERTY(EditDefaultsOnly, Category = "Manager Class")
   TSubclassOf<UEventManager> eventManagerClass;

   /**
    * TriggerManager - Handles activating and storing of trigger data across levels
    */
   UPROPERTY(EditDefaultsOnly, Category = "Manager Class")
   TSubclassOf<UTriggerManager> triggerManagerClass;

   /**
    * QuestManager - Handles everything quest related
    */
   UPROPERTY(EditDefaultsOnly, Category = "Manager Class")
   TSubclassOf<UQuestManager> questManagerClass;

   /**
    * MinigameManager - Handles starting and stopping of minigames
    */
   UPROPERTY(EditDefaultsOnly, Category = "Manager Class")
   TSubclassOf<UMinigameManager> minigameManagerClass;

   UPROPERTY(BlueprintGetter = GetEventManager)
   UEventManager* eventManager;

   UPROPERTY(BlueprintGetter = GetTriggerManager)
   UTriggerManager* triggerManager;

   UPROPERTY(BlueprintGetter = GetQuestManager)
   UQuestManager* questManager;

   UPROPERTY(BlueprintGetter = GetSaveManager)
   USaveLoadClass* saveLoadManager;

   UPROPERTY(BlueprintGetter = GetConditionalManager)
   UConditionalManager* conditionalManager;

   UPROPERTY(BlueprintGetter = GetMinigameManager)
   UMinigameManager* minigameManager;

   UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Callback")
   FOnLevelLoaded OnLevelLoaded;

   ///---Manager class accessors---
   UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
   UEventManager* GetEventManager() const { return eventManager; }

   UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
   UTriggerManager* GetTriggerManager() const { return triggerManager; }

   UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
   UQuestManager* GetQuestManager() const { return questManager; }

   UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
   USaveLoadClass* GetSaveManager() const { return saveLoadManager; }

   UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
   UConditionalManager* GetConditionalManager() const { return conditionalManager; }

   UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
   UMinigameManager* GetMinigameManager() const { return minigameManager; }

   ///---Level Things---
   /**
    * Gets the name of the current level that has been last streamed in
    */
   UFUNCTION(BlueprintCallable, Category = "RTSLevels")
   FORCEINLINE FString GetCurLevelName() const { return currentLevelName; }

   /**
    * Gets the name of the start level (main screen when game is loaded)
    */
   UFUNCTION(BlueprintCallable, Category = "Levels")
   FORCEINLINE FString GetStartingLvlName() const { return startingLevelName; }

   /**
    * Gets the name of the introduction level (first level in demo with Zone waking up in Chapter 2)
    */
   UFUNCTION(BlueprintCallable, Category = "Levels")
   FORCEINLINE FString GetSylphiaAptLvlName() const { return sylphiaApartment; }

   /**
    * Gets the name of the level of the area between the inner city and Zone's apartment zone
    */
   UFUNCTION(BlueprintCallable, Category = "Levels")
   FORCEINLINE FString GetZoneNeighborhoodLvlName() const { return roadToWubville; }

   /**
    * Gets the name of the blockaded inner city
    */
   UFUNCTION(BlueprintCallable, Category = "Levels")
   FORCEINLINE FString GetBlockadedCityLevelName() const { return blockadeCity; }

   /**
   * Gets the name of the factory where Zone realizes he's been living as if he's never existed this whole time
   */
   UFUNCTION(BlueprintCallable, Category = "Levels")
   FORCEINLINE FString GetFactoryLevelName() const { return factory; }

   /**Stream in a level and put in the loading screen*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "LevelLoading")
   void StreamLevelAsync(FName levelName);

   ///---Saving---
   /**
    * Saves the game in the SavedGames folder (in main project folder)
    * @param fileName - Filename of the new save file to be created
    */
   UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
   bool SaveGame(FString saveName);

   /**
    * Loads a game in the SavedGames folder (in main project folder)
    * @param fileName - Filename of save file to load
    */
   UFUNCTION(BlueprintCallable, Category = "Saving and Loading")
   bool LoadGame(FString fileName);

   ///---GamewideEvents---
   /**Trigger a gameover screeen*/
   UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "LevelLoading")
   void GameOver();
};
