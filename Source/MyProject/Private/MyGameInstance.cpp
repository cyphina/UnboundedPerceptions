#include "MyProject.h"
#include "UpResourceManager.h"
#include "MyGameInstance.h"
#include "WorldObjects/NPC.h"
#include "WorldObjects/IntimateNPC.h"
#include "Interactables/InteractableBase.h"
#include "Interactables/RTSDoor.h"

UMyGameInstance::UMyGameInstance()
{
}

void UMyGameInstance::Init()
{
   Super::Init();
}

void UMyGameInstance::Shutdown() { Super::Shutdown(); }

void UMyGameInstance::SaveLevelData(FName levelName)
{
   DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Up Level Saving"), STAT_StatsUpLevelSaving, STATGROUP_SaveAndLoading);
   DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Item Save Count"), STAT_StatsItemSaveCount, STATGROUP_SaveAndLoading);

<<<<<<< HEAD
   if (GetWorld()) {
      if (mapInfo.Contains(levelName)) // if we've already been here
=======
   if(GetWorld())
   {
      if(mapInfo.Contains(levelName)) // if we've already been here
>>>>>>> componentrefactor
      {
         FMapSaveInfo& mapData = mapInfo[levelName];
         mapData.pickupList.Empty();

         // Save NPCs and Interactables via overloaded functions which handles saving Pickups, Doors, and Trigger Decorators on Interactables.  Actor iterator
         // also iterates over subclasses

         for(TActorIterator<ANPC> actItr(GetWorld()); actItr; ++actItr)
         {
            (*actItr)->SaveNPCData(mapData);
            INC_DWORD_STAT(STAT_StatsItemSaveCount);
         }

         for(TActorIterator<AInteractableBase> actItr(GetWorld()); actItr; ++actItr)
         {
            (*actItr)->SaveInteractable(mapData);
            INC_DWORD_STAT(STAT_StatsItemSaveCount);
         }

         savedLevels.Add(levelName);
      }
   }
}

void UMyGameInstance::LoadLevelData(FName levelName)
{
   DECLARE_SCOPE_CYCLE_COUNTER(TEXT("Up Level Loading"), STAT_StatsUpLevelLoading, STATGROUP_SaveAndLoading);
   DECLARE_DWORD_ACCUMULATOR_STAT(TEXT("Item Load Count"), STAT_StatsItemLoadCount, STATGROUP_SaveAndLoading);

   if(GetWorld())
   {
      if(mapInfo.Contains(levelName))
      {
         FMapSaveInfo& mapData = mapInfo[levelName];

         // Load data for all NPCs including what topics were talked about, the NPCs default dialog, etc.
         for(TActorIterator<ANPC> actItr(GetWorld(), ANPC::StaticClass()); actItr; ++actItr)
         {
            (*actItr)->LoadNPCData(mapData);
            INC_DWORD_STAT(STAT_StatsItemLoadCount);
         }

         // Load interactable state (iterates through the different saved classes)
         for(TActorIterator<AInteractableBase> actItr(GetWorld(), AInteractableBase::StaticClass()); actItr; ++actItr)
         {
            (*actItr)->LoadInteractable(mapData);
            INC_DWORD_STAT(STAT_StatsItemLoadCount);
         }
      } else
      {
         // If we haven't traveled to this level yet then create an entry in our table for it so we can store data when we leave 
         mapInfo.Add(levelName, FMapSaveInfo());
      }
   }
}
