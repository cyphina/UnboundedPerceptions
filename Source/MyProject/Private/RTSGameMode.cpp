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

   // Call blueprint BeginPlay() afterwards
   Super::BeginPlay();
}

void ARTSGameMode::PostLogin(APlayerController* NewPlayer)
{
   Super::PostLogin(NewPlayer);
   __noop;
}

bool ARTSGameMode::SaveGame(FString saveName)
{
   return saveLoadManager->SaveToFilePath(FPaths::ProjectDir().Append("\\SavedGames\\" + saveName));
}

bool ARTSGameMode::LoadGame(FString fileName)
{
   bLoading                  = true;
   const bool successfulLoad = saveLoadManager->LoadFromFilePath(FPaths::ProjectDir().Append("\\SavedGames\\" + fileName));

   if(successfulLoad) saveLoadManager->SetupLoad();

   bLoading = false;
   return successfulLoad;
}
