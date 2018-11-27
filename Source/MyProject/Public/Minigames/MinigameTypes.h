#pragma once

#include "Trigger.h"

/**Type of minigame*/
UENUM(BlueprintType)
enum class EMinigameType : uint8 {
   DrawingMiniGame, /**game parameter is a filepath to the texture to be loaded and drawn*/
   Nurikabe         /**game parameter is a matrix with the game setup*/
};

/**Represents all the data a minigame needs.  The string parameters vary from minigames, but using strings we can start the minigame from a dialog*/
USTRUCT(BlueprintType, NoExport)
struct FMinigameData {
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
   TArray<FTriggerData> endTriggers;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
   TArray<FTriggerData> failTriggers;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Properties")
   TArray<FString> gameParameters;
};