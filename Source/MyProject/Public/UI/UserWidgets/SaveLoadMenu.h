// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "SaveLoadMenu.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API USaveLoadMenu : public UMyUserWidget
{
   GENERATED_BODY()

 public:
   /**Slot that is chosen to be saved/loaded*/
   UPROPERTY(BlueprintReadWrite, Category = "MenuData")
   int selectedSlot = -1;

   /**Lists all the save files as well as the creation time (as according to the OS filesystem)
    * @param fileNames: Pass in an array to have it filled with filenames.  Empties before adding filenames
    * @param fileCreationTimes: Pass in an array to have it filled with file creation times.  Empties before adding times
    */
   UFUNCTION(BlueprintCallable, Category = "MenuData")
   void GetSavedFileNamesAndTimes(UPARAM(ref) TArray<FString>& fileNames, UPARAM(ref) TArray<FDateTime>& fileCreationTimes);
};
