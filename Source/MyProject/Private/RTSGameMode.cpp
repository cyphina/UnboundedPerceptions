// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameMode.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/Trigger.h"
#include "Quests/QuestManager.h"
#include "EventSystem/RTSConditional.h"
#include "UserInput.h"

ARTSGameMode::ARTSGameMode() : Super()
{
	static ConstructorHelpers::FClassFinder<APawn> playerPawnClassFinder(TEXT("/Game/RTS_Tutorial/Blueprints/CameraPawn"));
	DefaultPawnClass = playerPawnClassFinder.Class;
}

void ARTSGameMode::BeginPlay()
{
	Super::BeginPlay(); 
	AUserInput* CPC = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());

	///Setup Manager Singletons
	eventManager = NewObject<UEventManager>(this, eventManagerClass, TEXT("EventManager"), RF_NoFlags);
	triggerManager = NewObject<UTriggerManager>(this, triggerManagerClass, TEXT("TriggerManager"), RF_NoFlags);
	questManager = NewObject<UQuestManager>(this, questManagerClass, TEXT("QuestManager"), RF_NoFlags);
	saveLoadManager = NewObject<USaveLoadClass>(this, TEXT("SaveManager"), RF_NoFlags);
	conditionalManager = NewObject<UConditionalManager>(this, TEXT("ConditionManager"), RF_NoFlags);

	///Initialize all their references
	eventManager->Init();
	triggerManager->Init();
	questManager->Init();
	saveLoadManager->Init();
	conditionalManager->Init();
}

bool ARTSGameMode::SaveGame(FString saveName)
{
	return saveLoadManager->SaveToFilePath(FPaths::ProjectDir().Append("\\SavedGames\\" + saveName));
}

bool ARTSGameMode::LoadGame(FString fileName)
{
	return saveLoadManager->LoadFromFilePath(FPaths::ProjectDir().Append("\\SavedGames\\" + fileName));
}
