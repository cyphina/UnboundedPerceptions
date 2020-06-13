// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "BreakMenu.generated.h"

/**
 * Widget class for widget that appears when we want to do things like save or quit
 */

class ARTSGameMode;
class UButton;

UCLASS()
class MYPROJECT_API UBreakMenu : public UMyDraggableWidget
{
   GENERATED_BODY()

 public:
   void NativeConstruct() override;

 private:
   UPROPERTY()
   class AHUDManager* hudManagerRef;

   ARTSGameMode* gameModeRef;

   UPROPERTY(Meta = (BindWidget))
   UButton* btnResume;
   UPROPERTY(Meta = (BindWidget))
   UButton* btnSaveLoad;
   UPROPERTY(Meta = (BindWidget))
   UButton* btnOption;
   UPROPERTY(Meta = (BindWidget))
   UButton* btnKeys;
   UPROPERTY(Meta = (BindWidget))
   UButton* btnExit;

   UFUNCTION(BlueprintCallable, Category = "BreakMenu Functionality")
   void Resume();

   UFUNCTION(BlueprintCallable, Category = "BreakMenu Functionality")
   void SaveLoad();

   UFUNCTION(BlueprintCallable, Category = "BreakMenu Functionality")
   void Options();

   UFUNCTION(BlueprintCallable, Category = "BreakMenu Functionality")
   void RemapKeys();

   UFUNCTION(BlueprintCallable, Category = "BreakMenu Functionality")
   void Exit();
};
