// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UpResourceManager.h"
#include "MyGameInstance.h"
#include "RTSGameState.h"
#include "WorldObjects/NPC.h"
#include "WorldObjects/IntimateNPC.h"
#include "Interactables/InteractableBase.h"
#include "Interactables/RTSDoor.h"
#include "Interactables/StorageContainer.h"
#include "Interactables/Pickup.h"

CSV_DECLARE_CATEGORY_EXTERN(UpLevelLoading);

UMyGameInstance::UMyGameInstance() {}

void UMyGameInstance::Init()
{
   Super::Init();
}

void UMyGameInstance::Shutdown() { Super::Shutdown(); }

void UMyGameInstance::SaveLevelData(FName levelName)
{
   CSV_SCOPED_TIMING_STAT(UpLevelLoading, MapSaveTime);

   if (GetWorld()) {
      if (mapInfo.Contains(levelName)) // if we've already been here
      {
         FMapSaveInfo& mapData = mapInfo[levelName];
         mapData.pickupList.Empty();

         // Save NPCs and Interactables via overloaded functions which handles saving Pickups, Doors, and Trigger Decorators on Interactables.  Actor iterator
         // also iterates over subclasses

         for (TActorIterator<ANPC> actItr(GetWorld()); actItr; ++actItr) {
            CSV_CUSTOM_STAT(UpLevelLoading, NPCSaveCount, 0, ECsvCustomStatOp::Accumulate);
            (*actItr)->SaveNPCData(mapData);
         }

         for (TActorIterator<AInteractableBase> actItr(GetWorld()); actItr; ++actItr) {
            CSV_CUSTOM_STAT(UpLevelLoading, InteractableSaveCount, 0, ECsvCustomStatOp::Accumulate);
            (*actItr)->SaveInteractable(mapData);
         }

         savedLevels.Add(levelName);
      }
   }
}

void UMyGameInstance::LoadLevelData(FName levelName)
{
   CSV_SCOPED_TIMING_STAT(UpLevelLoading, MapLoadTime);

   if (GetWorld()) {
      if (mapInfo.Contains(levelName)) {
         FMapSaveInfo& mapData = mapInfo[levelName];

         // Load data for all NPCs including what topics were talked about, the NPCs default dialog, etc.
         for (TActorIterator<ANPC> actItr(GetWorld(), ANPC::StaticClass()); actItr; ++actItr) {
            (*actItr)->LoadNPCData(mapData);
            CSV_CUSTOM_STAT(UpLevelLoading, NPCLoadCount, 0, ECsvCustomStatOp::Accumulate);
         }

         // Load interactable state (iterates through the different saved classes)
         for (TActorIterator<AInteractableBase> actItr(GetWorld(), AInteractableBase::StaticClass()); actItr; ++actItr) {
            (*actItr)->LoadInteractable(mapData);
            CSV_CUSTOM_STAT(UpLevelLoading, InteractableLoadCount, 0, ECsvCustomStatOp::Accumulate);
         }
      } else {
         // If we haven't traveled to this level yet then create an entry in our table for it so we can store data when we leave 
         mapInfo.Add(levelName, FMapSaveInfo());
      }
   }
}
