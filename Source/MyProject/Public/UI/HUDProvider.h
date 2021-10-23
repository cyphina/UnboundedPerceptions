// Created 8/29/20 7:18 PM

#pragma once
#include "HUDProvider.generated.h"

class URTSIngameWidget;
class UBreakMenu;
class USettingsMenu;
class UConfirmationBox;
class URTSInputBox;
class UStartMenu;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UHUDProvider : public UInterface
{
   GENERATED_BODY()
};

class IHUDProvider
{
   GENERATED_BODY()

public:
   UFUNCTION(BlueprintCallable, Category = "HUDManager")
   virtual URTSIngameWidget* GetIngameHUD() const = 0;

   UFUNCTION(BlueprintCallable, Category = "HUDManager")
   virtual UBreakMenu* GetBreakMenu() const = 0;

   UFUNCTION(BlueprintCallable, Category = "HUDManager")
   virtual USettingsMenu* GetSettingsMenu() const = 0;

   UFUNCTION(BlueprintCallable, Category = "HUDManager")
   virtual UConfirmationBox* GetConfirmationBox() const = 0;

   UFUNCTION(BlueprintCallable, Category = "HUDManager")
   virtual URTSInputBox* GetInputBox() const = 0;

   UFUNCTION(BlueprintCallable, Category = "HUDManager")
   virtual UStartMenu* GetStartMenu() const = 0;
};
