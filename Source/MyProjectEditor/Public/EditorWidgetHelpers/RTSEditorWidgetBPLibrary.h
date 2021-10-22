#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RTSEditorWidgetBPLibrary.generated.h"

class URTSTriggerBase;

/**
 * Holds functions that can be used with editor widgets.
 */
UCLASS()
class MYPROJECTEDITOR_API URTSEditorWidgetBPLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_BODY()

 public:
   /**
	 * @brief Used to get a list of all the triggers made in editor (across all assets).
	 * @return Returns all trigger objects found in the game.
	 */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "RTS Editor Helpers")
   static TArray<URTSTriggerBase*> GetTriggerObjects();
};
