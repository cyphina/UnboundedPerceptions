// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UI/HUDManager.h"
#include "ResourceManager.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/Trigger.h"
#include "Quests/QuestManager.h"
#include "UserInput.h"
#include "UI/UserWidgets/LoadingWidget.h"
#include "WorldObjects/Ally.h"
#include "MyGameInstance.h"

const FString UMyGameInstance::saveFilePath = FPaths::ProjectDir().Append("\\SavedGames\\");

UMyGameInstance::UMyGameInstance()
{
	
}

void UMyGameInstance::Init()
{
	Super::Init();
	//Set up resourcemanager globals
	ResourceManager::InitResourceManager();

	//Setup Manager Singletons
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	eventManager = NewObject<UEventManager>(this, TEXT("EventManager"), RF_NoFlags);
	triggerManager = NewObject<UTriggerManager>(this, TEXT("TriggerManager"), RF_NoFlags);
	//AUserInput* CPC = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()); WON'T WORK SINCE CPC Doesn't exist at Init time
	eventManager->Init();

	triggerManager->gameInstanceRef = this;
	questManager = NewObject<UQuestManager>(this, TEXT("QuestManager"), RF_NoFlags);

	saveLoadManager = NewObject<USaveLoadClass>(this, TEXT("SaveManager"), RF_NoFlags);

	eventManager->AddToRoot();
	questManager->AddToRoot();
	triggerManager->AddToRoot();
	saveLoadManager->AddToRoot();

	//Setup Post Async Level Load Callback
	//FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UMyGameInstance::OnWorldAdded);
	//!!!---Make sure this is set in the blueprints.  Can get removed after hotreload---!!!
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
	eventManager->RemoveFromRoot();
	questManager->RemoveFromRoot();
	triggerManager->RemoveFromRoot();
	saveLoadManager->RemoveFromRoot();
}

void UMyGameInstance::SetupManagerRefs(AUserInput* CPC)
{
	triggerManager->cpcRef = CPC;
	saveLoadManager->controllerRef = CPC;
	questManager->controllerRef = CPC;
}

void UMyGameInstance::OnFinishedStreamingLevel(const FName& packageName, UPackage* levelPackage, EAsyncLoadingResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("Level Streaming Finished!"));
	packageToBeLoaded = levelPackage;
	UWorld* world = UWorld::FindWorldInPackage(levelPackage);
	checkf(world != nullptr, TEXT("Error with loading level %s"), *packageName.ToString())
	worldBeingLoaded = world;
	//loadingWidget->OnLoadingFinished(levelPathToLoad.RightChop(numCharsInFilePathLocation));
}

void UMyGameInstance::OnWorldAdded(UWorld* world, const UWorld::InitializationValues values)
{
	if(worldBeingLoaded == nullptr)
		return;
	//check(worldBeingLoaded == world)

	worldBeingLoaded = nullptr;
	packageToBeLoaded = nullptr;
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, TEXT("World Loading Finished!"));
}

bool UMyGameInstance::SaveGame(FString saveName)
{
	return saveLoadManager->SaveToFilePath(FPaths::ProjectDir().Append("\\SavedGames\\" + saveName));
}

bool UMyGameInstance::LoadGame(FString fileName)
{
	return saveLoadManager->LoadFromFilePath(FPaths::ProjectDir().Append("\\SavedGames\\" + fileName));
}

//void UMyGameInstance::LoadLevelAsync(FString levelName) 
//{
//	loadingWidget->AddToViewport();
//	levelPathToLoad = "/Game/RTS_Tutorial/Maps/" + levelName;
//
//	Load via streaming
//	UGameplayStatics::LoadStreamLevel(this, *levelName, true, false, FLatentActionInfo());
//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::White, levelPathToLoad);
//	Load level w/o streaming
//	LoadPackageAsync(levelPathToLoad, FLoadPackageAsyncDelegate::CreateUObject(this, &UMyGameInstance::OnFinishedStreamingLevel));
//}
