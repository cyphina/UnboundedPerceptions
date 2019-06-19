#pragma once

#include "GameplayTags.h"
#include "SaveGameStructs.h"
#include "Interactables/InteractableBase.h"

/**Saves state of NPC before leaving level*/
struct FNPCSaveInfo {
   FText      name;
   FTransform transform;
   /**Should this actor even be spawned?  Maybe as part of a quest, this actor is somewhere else... at least temporarily*/
   bool enabled;

   bool  bWantsToConverse;
   FName conversationStarterName;
   FName defaultResponseName;

   TArray<FGameplayTag> conversationTopicKeys;
   TArray<FName>        conversationTopicValues;
   TSet<FName>          previousConversations;

   friend FArchive& operator<<(FArchive& ar, FNPCSaveInfo& saveData)
   {
      ar << saveData.name;
      ar << saveData.transform;
      ar << saveData.enabled;
      ar << saveData.bWantsToConverse;
      ar << saveData.conversationStarterName;
      ar << saveData.defaultResponseName;
      ar << saveData.conversationTopicKeys;
      ar << saveData.conversationTopicValues;
      ar << saveData.previousConversations;
      return ar;
   }
};

struct FNPCIntimateSaveInfo {
   FNPCSaveInfo npcInfo;
   int          relationshipPoints;
   int          currentRelationshipEventIndex;

   friend FArchive& operator<<(FArchive& ar, FNPCIntimateSaveInfo& saveData)
   {
      ar << saveData.npcInfo;
      ar << saveData.relationshipPoints;
      ar << saveData.currentRelationshipEventIndex;
      return ar;
   }
};

/**We only need to store trigger interactable save info since that's the only one with state that changes from its default, that is, when we load up a level, the
 *rest of the decorators are already set with the default values in the level editor, whereas a trigger may have values different from the default*/
struct FTriggerInteractableDecoratorSaveInfo {
   int  numCalls;
   bool enabled;
   FTriggerInteractableDecoratorSaveInfo(int numCalls, bool enabled) : numCalls{numCalls}, enabled{enabled} {}
   FTriggerInteractableDecoratorSaveInfo() = default; // makes this struct a POD

   friend FArchive& operator<<(FArchive& ar, FTriggerInteractableDecoratorSaveInfo& saveData)
   {
      ar << saveData.numCalls;
      ar << saveData.enabled;
      return ar;
   }
};

/** Save only details about interactables that have some kind of state (Pickup, Trigger, Door).  Spawner will create what it sees instead of drag dropping into world.*/
struct FInteractableSaveInfo {
   FTransform                                    transform;
   TArray<FTriggerInteractableDecoratorSaveInfo> triggerDecoratorInfo;
   TSubclassOf<AInteractableBase>                interactableClass;

   friend FArchive& operator<<(FArchive& ar, FInteractableSaveInfo& saveData)
   {
      ar << saveData.transform;
      ar << saveData.triggerDecoratorInfo;
      ar << saveData.interactableClass;
      return ar;
   }
};

struct FDoorInteractableSaveInfo {
   FInteractableSaveInfo interactableInfo;
   bool                  isLocked;

   friend FArchive& operator<<(FArchive& ar, FDoorInteractableSaveInfo& saveData)
   {
      ar << saveData.interactableInfo;
      ar << saveData.isLocked;
      return ar;
   }
};

struct FStorageContainerSaveInfo {
   FInteractableSaveInfo interactableInfo;
   FBackpackSaveInfo     backpackInfo;

   friend FArchive& operator<<(FArchive& ar, FStorageContainerSaveInfo& saveData)
   {
      ar << saveData.interactableInfo;
      ar << saveData.backpackInfo;
      return ar;
   }
};

/**Holds all the data that needs to be saved transitioning from map to map.*/
struct FMapSaveInfo {
   TArray<FInteractableSaveInfo>     interactablesInfo;
   TArray<FDoorInteractableSaveInfo> doorInteractables;
   TArray<FNPCSaveInfo>              npcsInfo;
   TArray<FNPCIntimateSaveInfo>      intimateNPCInfo;
   TSet<FString>                     pickupList;

   friend FArchive& operator<<(FArchive& ar, FMapSaveInfo& saveData)
   {
      ar << saveData.interactablesInfo;
      ar << saveData.doorInteractables;
      ar << saveData.npcsInfo;
      ar << saveData.intimateNPCInfo;
      ar << saveData.pickupList;
      return ar;
   }
};