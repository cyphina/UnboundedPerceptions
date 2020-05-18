// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "StartMenu.generated.h"

/**
 * UI for game's start menu with options like resuming, loading, changing in game options, seeing the credits, and quitting
 */

class ARTSGameMode;
class AUserInput;

UCLASS()
class MYPROJECT_API UStartMenu : public UUserWidget
{
   GENERATED_BODY()

   UPROPERTY()
   class AHUDManager* hudManagerRef;

 public:
   UPROPERTY(BlueprintReadOnly, Category = "References")
   ARTSGameMode* gameModeRef;

   UPROPERTY(BlueprintReadOnly, Category = "References")
   AUserInput* controllerRef;

   void NativeConstruct() override;

   /**Checks to see if a save file exists in the saved game folder
    *@param saveFileName: Name of the save file*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Saving")
   bool CheckIfSaveFileExists(FString saveFileName);

 private:
   UFUNCTION(BlueprintCallable, Category = "Saving")
   void StartGameLevelTransition();

   /**Helper function to setup all the preconditions for the game intro*/
   void GameEventStartSetup();
};
