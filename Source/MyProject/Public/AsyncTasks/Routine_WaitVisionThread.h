// Created 9/28/20 6:19 PM

#pragma once
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Routine_WaitVisionThread.generated.h"

/** Waits for the vision asynchronous work to finish up before transitioning to a new level.
  * If we don't do this we can get undefined behavior due to the threads iterating over actors
  * that have been GC'd.
 */

UCLASS()
class MYPROJECT_API URoutine_WaitVisionThread : public UBlueprintAsyncActionBase
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
   static URoutine_WaitVisionThread* WaitVisionAsync(UObject* WorldContextObject);

   void Activate() override;

 private:
   TWeakObjectPtr<UWorld> worldPtr;
   FTimerHandle visionCheckTimerHandle;

   void CheckVisionThreadsFinished();
};
