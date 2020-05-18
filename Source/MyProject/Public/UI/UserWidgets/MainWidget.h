// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "MainWidget.generated.h"

/**
 * Holds all the other widgets including widgets that can be opened from the main menu (settings) or other menus disabled during minigames
 */

class URTSIngameWidget;
class USettingsMenu;
class UBreakMenu;

UCLASS()
class MYPROJECT_API UMainWidget : public UMyUserWidget
{
   GENERATED_BODY()

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
   class AHUDManager* hudManagerRef;

#pragma region CallbacksAndReferences

 protected:
   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   URTSIngameWidget* ingameWidget;

   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   USettingsMenu* settingsMenu;

   UPROPERTY(BlueprintReadWrite, Meta = (BindWidget))
   UBreakMenu* breakMenu;

#pragma endregion
};
