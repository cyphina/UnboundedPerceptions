// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "SettingsMenu.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API USettingsMenu : public UMyUserWidget
{
   GENERATED_BODY()

   USettingsMenu();

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "SettingsMenu")
   int resolutionIndex = 0;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "SettingsMenu")
   int graphicQualityIndex = 1;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "SettingsMenu")
   int shadowQualityIndex = 1;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "SettingsMenu")
   int ppQualityIndex = 1;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "SettingsMenu")
   int aaQualityIndex = 1;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "SettingsMenu")
   int frameRateIndex = 1;

   ///< summary>We need to expose this to blueprints since we can't set FText in C++ since we can't have an array of references
   /// However, if we expose them to blueprints, their values as defined in the constructor line up properly...
   ///</summary>

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "SettingsMenu")
   TArray<FText> resolutionCategories;

   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "SettingsMenu")
   TArray<FText> qualityCategories;

   /**Make sure unlimited is last*/
   UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true), Category = "SettingsMenu")
   TArray<FText> fPSCategories;

   /**Helper function when toggling through quality settings via buttons
    * @param inc - Increased quality (true) or lowered quality (false)
    * @param qualityValue - Current quality index
    */
   void ChangeQualityValue(bool inc, UPARAM(ref) int& qualityValue, int numQualityVals);

 public:
   UFUNCTION(BlueprintCallable, Category = "Setings Change Functionality")
   void ChangeResolution(bool inc);

   UFUNCTION(BlueprintCallable, Category = "Setings Change Functionality")
   void ChangeGraphicsQuality(bool inc);

   UFUNCTION(BlueprintCallable, Category = "Setings Change Functionality")
   void ChangeShadowQuality(bool inc);

   UFUNCTION(BlueprintCallable, Category = "Setings Change Functionality")
   void ChangePostProcessingQuality(bool inc);

   UFUNCTION(BlueprintCallable, Category = "Setings Change Functionality")
   void ChangeAntiAliasingQuality(bool inc);

   UFUNCTION(BlueprintCallable, Category = "Setings Change Functionality")
   void ChangeFrameRateCap(bool inc);

   UFUNCTION(BlueprintCallable, Category = "Setings Change Functionality")
   void ChangeCameraSpeed(float val);

   UFUNCTION(BlueprintCallable, Category = "Setings Change Functionality")
   void ChangeAudioVolume(float val);

   UFUNCTION(BlueprintCallable, Category = "Setings Change Functionality")
   void ChangeEffectVolume(float val);
};
