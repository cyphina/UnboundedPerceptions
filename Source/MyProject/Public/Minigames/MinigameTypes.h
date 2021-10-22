#pragma once

#include "GameplayTagContainer.h"
class URTSTrigger;

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
   TArray<URTSTrigger*> endTriggers;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Minigame Properties")
   TArray<URTSTrigger*> failTriggers;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Minigame Properties")
   FGameplayTagContainer gameParameters;
};
