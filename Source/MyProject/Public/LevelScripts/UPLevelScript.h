#pragma once

#include "Engine/LevelScriptActor.h"
#include "EventSystem/Trigger.h"
#include "UPLevelScript.generated.h"

class AUserInput;
class ARTSGameState;

/**
 * Base script for all levels in UnboundedPreceptions
 * Includes a check to see if when a level script derived from this one is loaded up, to see if player loaded it or naturally transitioned to it, or if
 * moved to the level through gameplay
 * Includes time information for traveling between levels.  Maybe could use streaming instead.
 */
UCLASS()
class MYPROJECT_API AUPLevelScript : public ALevelScriptActor
{
   GENERATED_BODY()

 public:
   UPROPERTY(BlueprintReadWrite, Category = "OnLevelLoaded")
   TArray<FTriggerData> OnLevelLoadedTriggers;

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "References")
   AUserInput* GetControllerRef() const { return controllerRef; }

   void BeginPlay() override;

 protected:
   UPROPERTY(BlueprintReadOnly, Category = "References")
   AUserInput* controllerRef = nullptr;

 private:
   /**If time should pass entering/leaving or just chilling in the level*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
   bool shouldTimePass = false;

   /**How much time passes from entering/leaving*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
   FDateTime timeToPass;

   /** How far the camera can be panned in the -X and +X direction on this map. Specify values relative to origin (e.g., (550, 750) means we can move left 550 units max and up 750 units max*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
   FVector2D cameraBoundsX = FVector2D(1000,1000);

   /** How far the camera can be panned in the -Y and +Y direction on this map.  Specify values relative to origin (e.g., (550, 750) means we can move down 550 units max and up 750 units max*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
   FVector2D cameraBoundsY = FVector2D(1000,1000);

   /**What triggers should be run when this level is loaded
    * Should be saved
    */
   UPROPERTY(EditDefaultsOnly, Category = "Triggers")
   TArray<FTriggerData> triggersToRun;
};
