#include "MyProject.h"
#include "MinigameManager.h"
#include "RTSPawn.h"
#include "MinigamePawn.h"
#include "RTSGameMode.h"
#include "RTSGameState.h"
#include "UI/HUDManager.h"
#include "UserInput.h"

void UMinigameManager::Init()
{
}

void UMinigameManager::Auth_StartMiniGame(AUserInput* controllerRef, EMinigameType minigameType)
{
   if(controllerRef)
   {
      // Spawn the game pawn.  The game pawn will create a hud for itself
      switch(minigameType)
      {
         case EMinigameType::DrawingMiniGame: StartDrawingGame(controllerRef); break;
         default: break;
      }
      if(ARTSGameState* GS = Cast<ARTSGameState>(GetWorld()->GetGameState()))
      {
         GS->Client_StartMinigame(minigameType);
      }
   }
}

void UMinigameManager::StartTankGame(AUserInput* controllerRef)
{
}

void UMinigameManager::StartDrawingGame(AUserInput* controllerRef)
{
   AMinigamePawn* minigamePawn = controllerRef->GetWorld()->SpawnActorDeferred<AMinigamePawn>(drawingPawn.Get(), FTransform::Identity, controllerRef);
   UGameplayStatics::FinishSpawningActor(minigamePawn, FTransform::Identity);
   controllerRef->Possess(minigamePawn);
}

bool UMinigameManager::HasAuthority() const
{
   AActor* Outer = Cast<AActor>(GetOuter());
   check(Outer);
   return Outer->HasAuthority();
}

void UMinigameManager::EndMiniGame(AUserInput* controllerRef)
{
   if(controllerRef)
   {
      if(AMinigamePawn* minigamePawn = Cast<AMinigamePawn>(controllerRef->GetPawn()))
      {
         // Close remaining hud and repossess camerapawn
         controllerRef->Possess(controllerRef->GetCameraPawn());
         const bool bSuccess = minigamePawn->EndMinigame();

         if(bSuccess)
         {
            for(const FTriggerData& finishedTriggerActivation : minigamePawn->GetMinigameData().endTriggers)
            {
               controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
            }
         }
         else
         {
            for(const FTriggerData& finishedTriggerActivation : minigamePawn->GetMinigameData().failTriggers)
            {
               controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
            }
         }

         minigamePawn->Destroy();
         minigamePawn = nullptr;
         if(ARTSGameState* GS = Cast<ARTSGameState>(GetWorld()->GetGameState()))
         {
            GS->Client_EndMinigame(minigamePawn->GetMinigameData().minigameType);
         }
      }
   }
}

void UMinigameManager::AbortMinigame(AUserInput* controllerRef)
{
   if(AMinigamePawn* minigamePawn = Cast<AMinigamePawn>(controllerRef->GetPawn()))
   {
      minigamePawn->EndMinigame();
      minigamePawn->Destroy();
      minigamePawn = nullptr;

      if(ARTSGameState* GS = Cast<ARTSGameState>(GetWorld()->GetGameState()))
      {
         GS->Client_EndMinigame(minigamePawn->GetMinigameData().minigameType);
      }
   }
}
