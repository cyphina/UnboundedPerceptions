#include "MyProject.h"
#include "UPLevelScript.h"
#include "UserInput.h"
#include "RTSGameState.h"
#include "Quests/QuestManager.h"
#include "RTSTrigger.h"
#include "RTSGameMode.h"
#include "RTSPawn.h"
#include "SaveLoadClass.h"

void AUPLevelScript::ConstrainCameraPanLevelBounds(ARTSGameState* GS)
{
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

void AUPLevelScript::BeginPlay()
{
   if(const UWorld* World = GetWorld())
   {
      if(ARTSGameState* GS = Cast<ARTSGameState>(World->GetGameState()))
      {
         if(shouldTimePass)
         {
            GS->AddGameTime(FUpTime(timeToPass.GetSecond(), timeToPass.GetMinute(), timeToPass.GetHour()),
                            FUpDate(timeToPass.GetDay(), timeToPass.GetMonth(), timeToPass.GetYear()));
         }

         ConstrainCameraPanLevelBounds(GS);
      }

      if(AUserInput* PC = Cast<AUserInput>(World->GetFirstPlayerController()))
      {
         controllerRef = PC;
         if(controllerRef)
         {
            if(ARTSPawn* Pawn = controllerRef->GetCameraPawn())
            {
               Pawn->SetActorLocation(cameraStartLocation);
            }
            else
            {
               controllerRef->OnPlayerControllerSetup().AddDynamic(this, &AUPLevelScript::OnPlayerControllerSetup);
            }
         }
      }

      for(URTSTrigger* TriggerToRunOnLevelStart : triggersToRunOnLevelStart)
      {
         if(TriggerToRunOnLevelStart)
         {
            TriggerToRunOnLevelStart->TriggerEvent();
         }
      }
   }
   Super::BeginPlay();
}

void AUPLevelScript::OnPlayerControllerSetup()
{
   if(ARTSPawn* Pawn = controllerRef->GetCameraPawn())
   {
      Pawn->SetActorLocation(cameraStartLocation);
   }
}
