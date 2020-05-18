// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSGameMode.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/Trigger.h"
#include "Quests/QuestManager.h"
#include "EventSystem/RTSConditional.h"
#include "Minigames/MinigameManager.h"
#include "HUDManager.h"
#include "UserInput.h"
#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"

ARTSGameMode::ARTSGameMode() : Super()
{
}

void ARTSGameMode::BeginPlay()
{
   /// Setup Manager Singletons
   eventManager       = NewObject<UEventManager>(this, eventManagerClass, TEXT("EventManager"), RF_NoFlags);
   triggerManager     = NewObject<UTriggerManager>(this, triggerManagerClass, TEXT("TriggerManager"), RF_NoFlags);
   questManager       = NewObject<UQuestManager>(this, questManagerClass, TEXT("QuestManager"), RF_NoFlags);
   saveLoadManager    = NewObject<USaveLoadClass>(this, TEXT("SaveManager"), RF_NoFlags);
   conditionalManager = NewObject<UConditionalManager>(this, TEXT("ConditionManager"), RF_NoFlags);
   minigameManager    = NewObject<UMinigameManager>(this, minigameManagerClass, TEXT("MinigameManager"), RF_NoFlags);

   /// Initialize all their references
   eventManager->Init();
   triggerManager->Init();
   questManager->Init();
   saveLoadManager->Init();
   conditionalManager->Init();
   minigameManager->Init();

   /// HUDManager should get initialized first since the controller is initialized before the rules
   UObjectProperty* objectProperty = FindField<UObjectProperty>(minigameManager->GetClass(), "hudManagerRef");
   objectProperty->SetPropertyValue_InContainer(minigameManager, hudManagerRef);

   objectProperty = FindField<UObjectProperty>(questManager->GetClass(), "hudManagerRef");
   objectProperty->SetPropertyValue_InContainer(questManager, hudManagerRef);

   // Call blueprint BeginPlay() afterwards
   Super::BeginPlay();
}

bool ARTSGameMode::SaveGame(FString saveName)
{
   return saveLoadManager->SaveToFilePath(FPaths::ProjectDir().Append("\\SavedGames\\" + saveName));
}

bool ARTSGameMode::LoadGame(FString fileName)
{
   bLoading           = true;
   bool sucessfulLoad = saveLoadManager->LoadFromFilePath(FPaths::ProjectDir().Append("\\SavedGames\\" + fileName));

   if (sucessfulLoad) saveLoadManager->SetupLoad();

   bLoading = false;
   return sucessfulLoad;
}
