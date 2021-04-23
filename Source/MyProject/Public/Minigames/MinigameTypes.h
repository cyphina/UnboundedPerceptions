#pragma once

#include "GameplayTagContainer.h"
#include "Trigger.h"

/**Type of minigame*/
UENUM(BlueprintType)
enum class EMinigameType : uint8
{
   DrawingMiniGame, /**game parameter is a filepath to the texture to be loaded and drawn*/
   TankBattle,      /**game parameter will be some struct with enemy information*/
   Nurikabe         /**game parameter is a matrix with the game setup*/
};

/**Represents all the data a minigame needs.  The string parameters vary from minigames, but using strings we can start the minigame from a dialog*/
USTRUCT(BlueprintType, NoExport)
struct FMinigameData
{
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Minigame Properties")
   EMinigameType minigameType;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Minigame Properties")
   TArray<FTriggerData> endTriggers;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Minigame Properties")
   TArray<FTriggerData> failTriggers;
   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Minigame Properties")
   FGameplayTagContainer gameParameters;
};
