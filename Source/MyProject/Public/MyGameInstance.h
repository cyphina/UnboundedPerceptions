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
 */

UCLASS()
class MYPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()	
	//private constructor, don't make this bad boy
	UMyGameInstance(); 

	UPROPERTY(BlueprintGetter = GetEventManager)
	UEventManager*					eventManager;
	UPROPERTY(BlueprintGetter = GetTriggerManager)
	UTriggerManager*				triggerManager;
	UPROPERTY(BlueprintGetter = GetQuestManager)
	UQuestManager*					questManager;

	USaveLoadClass*					saveLoadManager;

	FString							levelPathToLoad;

	/**number of characters in the file path to get to the folder with all the maps*/
	const int						numCharsInFilePathLocation = 24;
	/**Helper function to save and load data since << operator is used for both depending on the archive type*/
	void							SaveLoadData(FArchive& ar);

public:
	
	static const FString			saveFilePath;

	/**Expose these classes so we can spawn a more derived blueprint class version of each manager in the code*/
	UPROPERTY(EditDefaultsOnly, Category = "Manager Class")
	TSubclassOf<UEventManager>		eventManagerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Manager Class")
	TSubclassOf<UTriggerManager>	triggerManagerClass;
	UPROPERTY(EditDefaultsOnly, Category = "Manager Class")
	TSubclassOf<UQuestManager>		questManagerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Loading Screen Class")
	TSubclassOf<ULoadingWidget>		loadingWidgetClass;

	UPROPERTY()
	UWorld*							worldBeingLoaded;

	UPROPERTY()
	UPackage*						packageToBeLoaded;

	UPROPERTY(BlueprintReadOnly)
	ULoadingWidget*					loadingWidget;

	/**GameInstance::Init is only called once, not per level loaded.  Called when play button is pressed.*/
	void							Init() override;
	void							Shutdown() override;
	void							SetupManagerRefs(AUserInput* CPC); //called by userinput

	/**Called when level package is finished loading*/
	void							OnFinishedStreamingLevel(const FName& packageName, UPackage* levelPackage, EAsyncLoadingResult::Type Result);
	/**Called when world is added*/
	void							OnWorldAdded(UWorld* world, const UWorld::InitializationValues values);
							
	///Manager class accessors///
	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
	UEventManager*					GetEventManager() const { return eventManager; }
	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
	UTriggerManager*				GetTriggerManager() const { return triggerManager; }
	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
	UQuestManager*					GetQuestManager() const { return questManager; }
	UFUNCTION(BlueprintGetter, BlueprintPure, Category = "Managers")
	USaveLoadClass*					GetSaveManager() const { return saveLoadManager; }

	UFUNCTION(BlueprintCallable, Category = "Managers")
	bool							SaveGame(FString saveName);
	UFUNCTION(BlueprintCallable, Category = "Managers")
	bool							LoadGame(FString fileName);

	/**Load another level's package asynchronously*/
	UFUNCTION(BlueprintCallable, Category = "LevelLoading")
	void							LoadLevelAsync(FString levelName);
	
};
