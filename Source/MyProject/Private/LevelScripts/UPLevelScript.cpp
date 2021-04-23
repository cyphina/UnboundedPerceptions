#include "MyProject.h"
#include "UPLevelScript.h"
#include "UserInput.h"
#include "RTSGameState.h"
#include "Quests/QuestManager.h"
#include "RTSGameMode.h"
#include "SaveLoadClass.h"

void AUPLevelScript::BeginPlay()
{
   controllerRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController());

   if(ARTSGameState* GS = Cast<ARTSGameState>(GetWorld()->GetGameState()))
   {
      if(shouldTimePass)
      {
         GS->AddGameTime(FUpTime(timeToPass.GetSecond(), timeToPass.GetMinute(), timeToPass.GetHour()),
                         FUpDate(timeToPass.GetDay(), timeToPass.GetMonth(), timeToPass.GetYear()));
      }

      if(cameraBoundsY.X >= 0 && cameraBoundsY.Y >= 0)
      {
         GS->SetCameraBoundX(cameraBoundsX);
      }
      else
      {
         UE_LOG(LogTemp, Warning, TEXT("Map boundaries are set to negative values. Change this in the level blueprint (if level bp is derived from UPLevelScript"));
      }

      if(cameraBoundsY.X >= 0 && cameraBoundsY.Y >= 0)
      {
         GS->SetCameraBoundY(cameraBoundsY);
      }
      else
      {
         UE_LOG(LogTemp, Warning, TEXT("Map boundaries are set to negative values. Change this in the level blueprint (if level bp is derived from UPLevelScript"));
      }
   }

   Super::BeginPlay();
}
