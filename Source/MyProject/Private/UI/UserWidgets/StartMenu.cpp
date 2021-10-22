#include "MyProject.h"
#include "StartMenu.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "MyGameInstance.h"
#include "SettingsMenu.h"
#include "RTSIngameWidget.h"
#include "DialogBox.h"
#include "RTSGameState.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/DEPRECATED_Trigger.h"

void UStartMenu::NativeConstruct()
{
   gameModeRef   = Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode());
   controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());

   // Add setting menu to overlay in blueprints
   Super::NativeConstruct(); // This calls blueprint Construct so call at end
}

bool UStartMenu::CheckIfSaveFileExists(FString saveFileName)
{
   FString        saveFilesPath = FPaths::ProjectDir().Append("\\SavedGames\\");
   IPlatformFile& platformFile  = FPlatformFileManager::Get().GetPlatformFile();

   if(!platformFile.DirectoryExists(*saveFilesPath))
   {
      platformFile.CreateDirectory(*saveFilesPath);
   }
   if(platformFile.FileExists(*(saveFilesPath + "\\" + saveFileName)))
   {
      return true;
   }
   return false;
}

void UStartMenu::StartGameLevelTransition()
{
   gameModeRef->StreamLevelAsync(*gameModeRef->GetSylphiaAptLvlName());

   // Renable ticks that are stopped in the level script
   controllerRef->SetActorTickEnabled(true);
   gameModeRef->SetActorTickEnabled(true);

   if(ARTSGameState* gameStateRef = Cast<ARTSGameState>(GetWorld()->GetGameState()))
   {
      gameStateRef->UpdateGameSpeed(2.f);
      gameStateRef->SetActorTickEnabled(true);
   }

   controllerRef->EnableInput(controllerRef);
   controllerRef->GetPawn()->EnableInput(controllerRef);
   // Change our game speed to default

   GameEventStartSetup();

   RemoveFromParent();
}

void UStartMenu::GameEventStartSetup()
{
   hudManagerRef->ShowDialogWithSource("SylphiaApartmentIntro", EDialogBoxCloseCase::none);
   gameModeRef->GetEventManager()->MoveToNextSection();

   FTriggerData addFirstQuestTrigger;
   addFirstQuestTrigger.enabled       = true;
   addFirstQuestTrigger.numCalls      = 1;
   addFirstQuestTrigger.triggerType   = ETriggerType::AddQuestTrigger;
   addFirstQuestTrigger.triggerValues = {"TutorialQuest", "1"};
   gameModeRef->GetTriggerManager()->ActivateTrigger(addFirstQuestTrigger);
}
