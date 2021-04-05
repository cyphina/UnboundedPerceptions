#pragma once

#include "Engine/GameInstance.h"
#include "LevelSaveStructs.h"
#include "MyGameInstance.generated.h"

DECLARE_STATS_GROUP(TEXT("Save and Loading"), STATGROUP_SaveAndLoading, STATCAT_Advanced);

class UEventManager;
class AHUDManager;
class UTriggerManager;
class UQuestManager;
class ULoadingWidget;

/**
 * GameInstance information kept across levels.  Does not make ACTORS persistent, only data
 * However GameInstance itself is persistent
 * Will be used to save data about state of each map (for now we'll cache this data) since that data needs to be stored in memory in case player returns to a place
 */

UCLASS()
class MYPROJECT_API UMyGameInstance : public UGameInstance
{
   GENERATED_BODY()

   UMyGameInstance();

 public:

   /**
    *GameInstance::Init is only called once, not per level loaded.  Called when play button is pressed.
    */
   /**List of all the details about the map that need to be saved across changing levels, including levels not even on the screen*/
   TMap<FName, FMapSaveInfo> mapInfo;

   /**Cache what levels have been saved so when we save the information to disk, we don't have to save information about every level*/
   TSet<FName> savedLevels;

   void Init() override;
   void Shutdown() override;

   /**We only save the full details on things that can be destroyed completely in a level since it will already be spawned by the time we try to load, so all we have to do
    *is save the modifyable properties (state) to be loaded. This saves some memory at the cost of performance since we have to search for the object, rather than
    *just having a reference when the object is spawned, which we would have if we spawned it manually using the spawner technique
    *
    *This technique does have the flaw that we can't as easily spawn new objects in a level that is different from the one that we're in
    */
   UFUNCTION(BlueprintCallable, Category = "Level Save/Load")
   void SaveLevelData(FName levelName);

   /**Call this after the level is loaded*/
   UFUNCTION(BlueprintCallable, Category = "Level Save/Load")
   void LoadLevelData(FName levelName);
};
