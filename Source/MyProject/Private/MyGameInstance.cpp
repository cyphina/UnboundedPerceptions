// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ResourceManager.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/Trigger.h"
#include "Quests/QuestManager.h"
#include "UserInput.h"
#include "UI/UserWidgets/LoadingWidget.h"
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

	//Setup Post Async Level Load Callback
	//FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UMyGameInstance::OnWorldAdded);
	//!!!---Make sure this is set in the blueprints.  Can get removed after hotreload---!!!
}

void UMyGameInstance::Shutdown()
{
	Super::Shutdown();
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
