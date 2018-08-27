#pragma once

#include "GameplayTags.h"
#include "SaveGameStructs.h"
#include "Interactables/InteractableBase.h"

/**Saves state of NPC before leaving level*/
struct FNPCSaveInfo
{
	FText							name;
	FTransform						transform;
	/**Should this actor even be spawned?  Maybe as part of a quest, this actor is somewhere else... at least temporarily*/
	bool							enabled;

	bool							bWantsToConverse;
	FName							conversationStarterName;
	FName							defaultResponseName;

	TArray<FGameplayTag>			conversationTopicKeys;
	TArray<FName>					conversationTopicValues;
	TSet<FName>						previousConversations;
};

struct FNPCIntimateSaveInfo
{
	FNPCSaveInfo					npcInfo;
	int								relationshipPoints;
	int								currentRelationshipEventIndex;
};

/**We only need to store trigger interactable save info since that's the only one with state that changes from its default, that is, when we load up a level, the 
 *rest of the decorators are already set with the default values in the level editor, whereas a trigger may have values different from the default*/
struct FTriggerInteractableDecoratorSaveInfo
{
	int						numCalls;
	bool					enabled;
	FTriggerInteractableDecoratorSaveInfo(int numCalls, bool enabled) : numCalls{numCalls}, enabled{enabled} {}
	FTriggerInteractableDecoratorSaveInfo() = default; //makes this struct a POD
};

/** Save only details about interactables that have some kind of state (Pickup, Trigger, Door).  Spawner will create what it sees instead of drag dropping into world.*/
struct FInteractableSaveInfo
{
	FTransform											transform;
	TArray<FTriggerInteractableDecoratorSaveInfo>		triggerDecoratorInfo;
	TSubclassOf<AInteractableBase>						interactableClass;
};

struct FDoorInteractableSaveInfo
{
	FInteractableSaveInfo						interactableInfo;
	bool										isLocked;
};

struct FStorageContainerSaveInfo
{
	FInteractableSaveInfo						interactableInfo;
	FBackpackSaveInfo							backpackInfo;
};

/**Holds all the data that needs to be saved transitioning from map to map.*/
struct FMapSaveInfo
{
	TArray<FInteractableSaveInfo>				interactablesInfo;
	TArray<FDoorInteractableSaveInfo>			doorInteractables;
	TArray<FNPCSaveInfo>						npcsInfo;
	TArray<FNPCIntimateSaveInfo>				intimateNPCInfo;
	TSet<FString>								pickupList;
};