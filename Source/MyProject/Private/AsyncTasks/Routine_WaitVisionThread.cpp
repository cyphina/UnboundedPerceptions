// Created 9/28/20 6:28 PM

#include "MyProject.h"
#include "Routine_WaitVisionThread.h"

#include "VisionContext.h"

URoutine_WaitVisionThread* URoutine_WaitVisionThread::WaitVisionAsync(UObject* worldContextObject)
{
   UWorld* world = GEngine->GetWorldFromContextObject(worldContextObject);
   if(!world) return nullptr;

   URoutine_WaitVisionThread* action = NewObject<URoutine_WaitVisionThread>();
   action->worldPtr                  = world;
   action->RegisterWithGameInstance(world);
   return action;
}

void URoutine_WaitVisionThread::Activate()
{
   if(UWorld* world = worldPtr.Get()) {
      world->GetTimerManager().SetTimer(visionCheckTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::CheckVisionThreadsFinished), 0.1f, true);
      return;
   }
   SetReadyToDestroy();
}

void URoutine_WaitVisionThread::CheckVisionThreadsFinished()
{
   if(worldPtr.IsValid()) {
      auto visionContext = Cast<IVisionContext>(worldPtr->GetGameState());
      if(visionContext->GetVisiblePlayerUnits().Num() == 0 && visionContext->GetVisibleEnemies().Num() == 0) SetReadyToDestroy();
   } else
      SetReadyToDestroy();
}
