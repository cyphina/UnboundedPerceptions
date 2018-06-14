#pragma once

#include "Engine/GameInstance.h"
#include "SaveLoadClass.h"
#include "MyGameInstance.generated.h"

class UEventManager;
class AHUDManager;
class UTriggerManager;
class UQuestManager;
class ULoadingWidget;

/**
 * GameInstance information kept across levels.  Does not make ACTORS persistant, only data
 * However GameInstance itself is persistent
 * Stores client data
 */

UCLASS()
class MYPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()	
	//private constructor, don't make this bad boy
	UMyGameInstance(); 

	FString							levelPathToLoad;

	/**number of characters in the file path to get to the folder with all the maps*/
	const int						numCharsInFilePathLocation = 24;

public:
	
	static const FString			saveFilePath;

	

	/*UPROPERTY(EditDefaultsOnly, Category = "Loading Screen Class")
	TSubclassOf<ULoadingWidget>		loadingWidgetClass;*/

	UPROPERTY()
	UWorld*							worldBeingLoaded;

	UPROPERTY()
	UPackage*						packageToBeLoaded;

	/**
	 *GameInstance::Init is only called once, not per level loaded.  Called when play button is pressed.
	 */
	void							Init() override;
	void							Shutdown() override;

	/**
	 *Called when level package is finished loading.  Deprecated since we now use level streaming
	 */
	void							OnFinishedStreamingLevel(const FName& packageName, UPackage* levelPackage, EAsyncLoadingResult::Type Result);

	/**
	 *Called when world is added
	 */
	void							OnWorldAdded(UWorld* world, const UWorld::InitializationValues values);

	/**Load another level's package asynchronously*/
	//UFUNCTION(BlueprintCallable, Category = "LevelLoading")
	//void							LoadLevelAsync(FString levelName);
	
};
