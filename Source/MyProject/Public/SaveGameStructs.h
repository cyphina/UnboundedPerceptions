#pragma once

#include "GameplayTags.h"
#include "Archive.h"

/*Definitions of all the structs which we first copy the data we want to save to
 The structs are declared here since we want them to be class members since we need to setup a callback without any parameters
to FWorldDelegate::OnPostWorldInitialization to properly call the loading setup at the right time since OpenLevel is Async.*/

struct FSaveGameDataInfo {
   FName     id;        // Player can name a save
   FDateTime timestamp; // time when game is saved

   friend FArchive& operator<<(FArchive& ar, FSaveGameDataInfo& saveDataInfo)
   {
      ar << saveDataInfo.id;
      ar << saveDataInfo.timestamp;
      return ar;
   }
};

// LevelData - Will contain information about level state
struct FSceneSaveInfo {
   FName         levelName;
   TArray<FName> pickedUpInteractables; // what items have been picked up already? - This may be saved in a struct with all the level data

   friend FArchive& operator<<(FArchive& ar, FSceneSaveInfo& saveData)
   {
      ar << saveData.levelName;
      ar << saveData.pickedUpInteractables;
      return ar;
   }
};

struct FCameraSaveInfo {
   FTransform cameraTransform;
   int        cameraSpeed;
   bool       isCamNavDisabled;

   friend FArchive& operator<<(FArchive& ar, FCameraSaveInfo& saveData)
   {
      ar << saveData.cameraTransform;
      ar << saveData.cameraSpeed;
      ar << saveData.isCamNavDisabled;
      return ar;
   }

   friend void operator<<(FMemoryReader& ar, FCameraSaveInfo& saveData)
   {
      ar << saveData.cameraTransform;
      ar << saveData.cameraSpeed;
      ar << saveData.isCamNavDisabled;
   }
};

struct FBasePlayerSaveInfo {
   int           money;
   TArray<FName> dialogTopics;
   int           heroNum;
   int           summonNum;
   int           npcNum;

   friend FArchive& operator<<(FArchive& ar, FBasePlayerSaveInfo& saveData)
   {
      ar << saveData.money;
      ar << saveData.dialogTopics;
      ar << saveData.heroNum << saveData.summonNum << saveData.npcNum;
      return ar;
   }
};

struct FBaseCharacterSaveInfo {
   TArray<int> attributes;
   TArray<int> skills;
   TArray<int> vitals;
   TArray<int> currentVitals;
   TArray<int> mechanics;
   int         level;

   friend FArchive& operator<<(FArchive& ar, FBaseCharacterSaveInfo& saveData)
   {
      ar << saveData.attributes;
      ar << saveData.skills;
      ar << saveData.vitals;
      ar << saveData.mechanics;
      ar << saveData.level;
      return ar;
   }
};

struct FAllySaveInfo {
   FText                  name;
   FTransform             actorTransform;
   FBaseCharacterSaveInfo baseCSaveInfo;
   // TODO: Save buffs and debuffs

   friend FArchive& operator<<(FArchive& ar, FAllySaveInfo& saveData)
   {
      ar << saveData.name;
      ar << saveData.actorTransform;
      ar << saveData.baseCSaveInfo;
      return ar;
   }
};

struct FSummonSaveInfo {
   FAllySaveInfo allyInfo;
   int           duration;

   friend FArchive& operator<<(FArchive& ar, FSummonSaveInfo& saveData)
   {
      ar << saveData.allyInfo;
      ar << saveData.duration;
      return ar;
   }
};

struct FBackpackSaveInfo {
   TArray<int> itemIDs;
   TArray<int> itemCounts;
   TArray<int> itemSlots;

   friend FArchive& operator<<(FArchive& ar, FBackpackSaveInfo& saveData)
   {
      ar << saveData.itemIDs;
      ar << saveData.itemCounts;
      ar << saveData.itemSlots;
      return ar;
   }
};

struct FSpellbookSaveInfo
{
   TArray<int> learnedSpellIDs;
   TArray<int> learnedSpellLevels;

   friend FArchive& operator<<(FArchive& ar, FSpellbookSaveInfo& saveData)
   {
      ar << saveData.learnedSpellIDs;
      ar << saveData.learnedSpellLevels;
      return ar;
   }
};

struct FHeroSaveInfo {
   FAllySaveInfo     allyInfo;
   int               currentExp;
   int               expToNextLevel; // we could technically recalculate this...
   int               attPoints;
   int               skillPoints;
   TArray<int>       spellIDs;
   FBackpackSaveInfo backpackInfo;

   friend FArchive& operator<<(FArchive& ar, FHeroSaveInfo& saveData)
   {
      ar << saveData.allyInfo;
      ar << saveData.currentExp;
      ar << saveData.expToNextLevel;
      ar << saveData.attPoints;
      ar << saveData.skillPoints;
      ar << saveData.spellIDs;
      ar << saveData.backpackInfo;
      return ar;
   }
};

// TODO: In progress
struct FQuestSaveInfo {
   TArray<int>    currentGoalIndices;
   TMap<int, int> currentAmounts;
   FText          currentDescription;
};

struct FQuestManagerSaveInfo {
   TArray<FGameplayTag> currentQuestIDs;
   TArray<FGameplayTag> completedQuestIDs;
   FGameplayTag         currentlySelectedQuest;
   /**What is currently being displayed on the questListWidget?*/
   TArray<FGameplayTag> currentlyTrackedQuests;
};
