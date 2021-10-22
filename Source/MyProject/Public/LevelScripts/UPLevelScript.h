#pragma once

#include "Engine/LevelScriptActor.h"
#include "EventSystem/DEPRECATED_Trigger.h"
#include "UPLevelScript.generated.h"

class AUserInput;
class ARTSGameState;
class URTSTrigger;

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
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "References")
   AUserInput* GetControllerRef() const { return controllerRef; }

   void BeginPlay() override;

   /**
    * Should be saved
    */
   UPROPERTY(EditDefaultsOnly, Instanced, Category = "Triggers")
   TArray<URTSTrigger*> triggersToRunOnLevelStart;

   FVector GetCameraStartLocation() const { return cameraStartLocation; }

   TPair<FVector2D, FVector2D> GetCameraBounds() const { return TPair<FVector2D, FVector2D>{cameraBoundsX, cameraBoundsY}; }

 protected:
   UPROPERTY(BlueprintReadOnly, Category = "References")
   AUserInput* controllerRef = nullptr;

 private:
   UFUNCTION()
   void OnPlayerControllerSetup();

   void ConstrainCameraPanLevelBounds(ARTSGameState* GS);

   /**If time should pass entering/leaving or just chilling in the level*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
   bool shouldTimePass = false;

   /**How much time passes from entering/leaving */
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
   FDateTime timeToPass;

   /** Where the camera pawn should start at */
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true, MakeEditWidget = true), Category = "DefaultValues")
   FVector cameraStartLocation = FVector::ZeroVector;

   /** How far the camera can be panned in the -X and +X direction on this map. Specify values relative to origin (e.g., (550, 750) means we can move left 550 units max and up 750 units max*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
   FVector2D cameraBoundsX = FVector2D(1000, 1000);

   /** How far the camera can be panned in the -Y and +Y direction on this map.  Specify values relative to origin (e.g., (550, 750) means we can move down 550 units max and up 750 units max*/
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "DefaultValues")
   FVector2D cameraBoundsY = FVector2D(1000, 1000);
};
