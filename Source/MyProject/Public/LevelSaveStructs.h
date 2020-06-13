#pragma once

/** This header has information used to save data between level transitions in memory
 * Saves information about a few levels so that loading can be shorter.
 */

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

struct FNPCIntimateSaveInfo : FNPCSaveInfo {
   int relationshipPoints;
   int currentRelationshipEventIndex;

   friend FArchive& operator<<(FArchive& ar, FNPCIntimateSaveInfo& saveData)
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

      ar << saveData.relationshipPoints;
      ar << saveData.currentRelationshipEventIndex;
      return ar;
   }
};

template <typename ElementType>
struct NPCSaveKeyFuncs : DefaultKeyFuncs<ElementType, true> {

   using KeyInitType = typename TCallTraits<ElementType>::ParamType;

   /** Calculates a hash index for a key. */
   static FORCEINLINE uint32 GetKeyHash(KeyInitType Key) { return GetTypeHash(Key.name.ToString()) + GetTypeHash(Key.transform.GetLocation()); }

   /** Used when trying to ensure bucket has correct value in hashtable.  Matches the interactable actor with the struct holding its data.*/
   template <typename ComparableKey>
   static FORCEINLINE bool Matches(KeyInitType A, ComparableKey B) { return A.name.ToString() == B->GetGameName().ToString(); }

   /** Overload used when adding values */
   static FORCEINLINE bool Matches(KeyInitType& A, KeyInitType& B) { return A.name.ToString() == B.name.ToString(); }
};

/**We only need to store trigger interactable save info since that's the only one with state that changes from its default, that is, when we load up a level, the
 *rest of the decorators are already set with the default values in the level editor, whereas a trigger may have values different from the default*/
struct FTriggerInteractableDecoratorSaveInfo {
   int  numCalls;
   bool enabled;

   FTriggerInteractableDecoratorSaveInfo(int numCalls, bool enabled) :
      numCalls{numCalls}, enabled{enabled} {}

   FTriggerInteractableDecoratorSaveInfo() = default; // makes this struct a POD

   friend FArchive& operator<<(FArchive& ar, FTriggerInteractableDecoratorSaveInfo& saveData)
   {
      ar << saveData.numCalls;
      ar << saveData.enabled;
      return ar;
   }
};

/** Save only details about interactables that have some kind of state (Pickup, Trigger, Door).  Spawner will create what it sees instead of drag dropping into world.
 * Do not directly use this class, instead use the wrapper.
 */
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

// Created so I can use the template function I created on a FInteractableSaveInfo
struct FInteractableSaveInfoWrapper {

   FInteractableSaveInfo interactableInfo;

   friend FArchive& operator<<(FArchive& ar, FInteractableSaveInfoWrapper& saveData)
   {
      ar << saveData.interactableInfo;
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

template <typename ElementType>
struct InteractableSaveKeyFuncs : DefaultKeyFuncs<ElementType, true> {

   using KeyInitType = typename TCallTraits<ElementType>::ParamType ;
   using ElementInitType = typename TCallTraits<ElementType>::ParamType ;

   /** Calculates a hash index for a key. */
   static FORCEINLINE uint32 GetKeyHash(KeyInitType& Key) { return GetTypeHash(Key.interactableInfo.transform.GetLocation()); }

   /** Used when trying to ensure bucket has correct value in hashtable.  Matches the interactable actor with the struct holding its data.*/
   template <typename ComparableKey>
   static FORCEINLINE bool Matches(KeyInitType& A, ComparableKey& B) { return A.interactableInfo.transform.GetLocation() == B->GetActorLocation(); }

   /** Overload used when adding values */
   static FORCEINLINE bool Matches(KeyInitType& A, KeyInitType& B) { return A.interactableInfo.transform.GetLocation() == B.interactableInfo.transform.GetLocation(); }
};

/**Holds all the data that needs to be saved transitioning from map to map.
 *By holding specific data for types of interactables, we can reduce the time it takes to search through the data to*/
struct FMapSaveInfo {
   TSet<FInteractableSaveInfoWrapper, InteractableSaveKeyFuncs<FInteractableSaveInfoWrapper>> interactablesInfo;
   TSet<FDoorInteractableSaveInfo, InteractableSaveKeyFuncs<FDoorInteractableSaveInfo>>       doorInteractables;
   TSet<FStorageContainerSaveInfo, InteractableSaveKeyFuncs<FStorageContainerSaveInfo>>       wardrobeInteractables;
   TSet<FNPCSaveInfo, NPCSaveKeyFuncs<FNPCSaveInfo>>                                          npcsInfo;
   TSet<FString>                                                                              pickupList;

   friend FArchive& operator<<(FArchive& ar, FMapSaveInfo& saveData)
   {
      ar << saveData.interactablesInfo;
      ar << saveData.doorInteractables;
      ar << saveData.npcsInfo;
      ar << saveData.pickupList;
      return ar;
   }
};
