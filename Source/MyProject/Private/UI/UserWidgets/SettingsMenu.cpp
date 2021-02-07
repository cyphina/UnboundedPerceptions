// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "SettingsMenu.h"
#include "RTSPawn.h"
#include "UserInput.h"
#include "MyGameInstance.h"
#include "UIDelegateContext.h"

USettingsMenu::USettingsMenu() : Super()
{
#define LOCTEXT_NAMESPACE "SettingsMenuCategories"

   resolutionCategories = {LOCTEXT("Resolution1", "1920x1080"), LOCTEXT("Resolution2", "1600x900"), LOCTEXT("Resolution3", "1366x768"),
                           LOCTEXT("Resolution4", "1024x768"), LOCTEXT("Resolution5", "800x600")};
   qualityCategories    = {LOCTEXT("Quality1", "Low"), LOCTEXT("Quality2", "Medium"), LOCTEXT("Quality3", "High"), LOCTEXT("Quality4", "Epic"),
                        LOCTEXT("Quality5", "Cinematic")};
   fPSCategories = {LOCTEXT("FPS1", "30"), LOCTEXT("FPS2", "60"), LOCTEXT("FPS3", "90"), LOCTEXT("FPS4", "120"), LOCTEXT("FPS5", "200"), LOCTEXT("FPS6", "Unlimited")};

   resolutionIndex = 0;
#undef LOCTEXT_NAMESPACE
}

void USettingsMenu::ChangeQualityValue(bool inc, UPARAM(ref) int& qualityValue, int numQualityVals)
{
   if(inc) {
      if(qualityValue < numQualityVals - 1)
         ++qualityValue;
   } else {
      if(qualityValue > 0)
         --qualityValue;
   }
}

void USettingsMenu::ChangeResolution(bool inc)
{
   ChangeQualityValue(inc, resolutionIndex, resolutionCategories.Num());
   FString command = "r.SetRes " + resolutionCategories[resolutionIndex].ToString() + "w";
   CPC->ConsoleCommand(command);
}

void USettingsMenu::ChangeGraphicsQuality(bool inc)
{
   ChangeQualityValue(inc, graphicQualityIndex, qualityCategories.Num());
   FString command = "sg.TextureQuality " + FString::FromInt(graphicQualityIndex);
   CPC->ConsoleCommand(command);
}

void USettingsMenu::ChangeShadowQuality(bool inc)
{
   ChangeQualityValue(inc, shadowQualityIndex, qualityCategories.Num());
   FString command = "sg.ShadowQuality " + FString::FromInt(shadowQualityIndex);
   CPC->ConsoleCommand(command);
}

void USettingsMenu::ChangePostProcessingQuality(bool inc)
{
   ChangeQualityValue(inc, ppQualityIndex, qualityCategories.Num());
   FString command = "sg.PostProcessQuality " + FString::FromInt(ppQualityIndex);
   CPC->ConsoleCommand(command);
}

void USettingsMenu::ChangeAntiAliasingQuality(bool inc)
{
   ChangeQualityValue(inc, aaQualityIndex, qualityCategories.Num());
   FString command = "sg.AntiAliasingQuality " + FString::FromInt(aaQualityIndex);
   CPC->ConsoleCommand(command);
}

void USettingsMenu::ChangeFrameRateCap(bool inc)
{
   ChangeQualityValue(inc, frameRateIndex, fPSCategories.Num());
   FString command;
   if(frameRateIndex != fPSCategories.Num() - 1)
      command = "t.MaxFPS " + fPSCategories[frameRateIndex].ToString();
   else
      command = "t.MaxFPS 0";
   CPC->ConsoleCommand(command);
}

void USettingsMenu::ChangeCameraSpeed(float val)
{
   CPC->GetCameraPawn()->SetCamMoveSpeedMultiplier(val);
}

void USettingsMenu::ChangeAudioVolume(float val)
{
   // TODO: Implement any sounds haha
}

void USettingsMenu::ChangeEffectVolume(float val)
{
   // TODO: Implement any sounds haha
}

void USettingsMenu::ToggleQuickCast()
{
  GetOwningLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnQuickCastSettingToggled().Broadcast();
}

void USettingsMenu::ToggleStaticFormation()
{
   GetOwningLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnStaticFormationToggled().Broadcast();
}

void USettingsMenu::ToggleAutoClick()
{
   GetOwningLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnAutoclickToggled().Broadcast();
}
