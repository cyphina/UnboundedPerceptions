// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ResourceManager.h"
#include "MyGameInstance.h"
#include "WorldObjects/NPC.h"
#include "WorldObjects/IntimateNPC.h"
#include "Interactables/InteractableBase.h"
#include "Interactables/RTSDoor.h"
#include "Interactables/StorageContainer.h"
#include "Interactables/Pickup.h"


UMyGameInstance::UMyGameInstance()
{

}

void UMyGameInstance::Init()
{
	Super::Init();
	//Set up resourcemanager globals
	ResourceManager::InitResourceManager();
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
}

void UMyGameInstance::SaveLevelData(FName levelName)
{
	if (GetWorld())
	{
		if (mapInfo.Contains(levelName)) //if we've already been here
		{
			FMapSaveInfo& mapData = mapInfo[levelName];
			mapData.pickupList.Empty();

			for (TActorIterator<ANPC> actItr(GetWorld()); actItr; ++actItr)
			{
				(*actItr)->SaveNPCData(mapData);
			}

			for (TActorIterator<AInteractableBase> actItr(GetWorld()); actItr; ++actItr)
			{
				(*actItr)->SaveInteractable(mapData);
			}
		}
	}
}

void UMyGameInstance::LoadLevelData(FName levelName)
{
	if (GetWorld())
	{
		if (mapInfo.Contains(levelName))
		{
			FMapSaveInfo& mapData = mapInfo[levelName];

			for (FDoorInteractableSaveInfo doorInfo : mapData.doorInteractables)
			{
				for (TActorIterator<ARTSDoor> actItr(GetWorld()); actItr; ++actItr)
				{
					if ((*actItr)->GetTransform().Equals(doorInfo.interactableInfo.transform))
					{
						(*actItr)->LoadInteractable(doorInfo);
						break;
					}
				}
			}

			for (FNPCIntimateSaveInfo intimateNPCInfo : mapData.intimateNPCInfo)
			{
				for (TActorIterator<AIntimateNPC> actItr(GetWorld()); actItr; ++actItr)
				{
					if ((*actItr)->GetGameName().EqualTo(intimateNPCInfo.npcInfo.name))
					{
						(*actItr)->LoadNPCData(intimateNPCInfo);
						break;
					}
				}
			}

			for (FNPCSaveInfo npcInfo : mapData.npcsInfo)
			{
				for (TActorIterator<ANPC> actItr(GetWorld()); actItr; ++actItr)
				{
					if ((*actItr)->GetGameName().EqualTo(npcInfo.name))
					{
						(*actItr)->LoadNPCData(npcInfo);
						break;
					}
				}
			}

			for (FInteractableSaveInfo interactableInfo : mapData.interactablesInfo)
			{
				for (TActorIterator<AInteractableBase> actItr(GetWorld(), interactableInfo.interactableClass); actItr; ++actItr)
				{
					if ((*actItr)->GetTransform().Equals(interactableInfo.transform))
					{
						(*actItr)->LoadInteractable(interactableInfo);
						break;
					}
				}
			}

			for (TActorIterator<APickup> actItr(GetWorld()); actItr; ++actItr)
			{
				if (!mapData.pickupList.Contains((*actItr)->GetName()))
				{
					(*actItr)->Destroy();
				}
			}
		}
		else
		{
			mapInfo.Add(levelName,FMapSaveInfo());
			return;
		}
	}
}



