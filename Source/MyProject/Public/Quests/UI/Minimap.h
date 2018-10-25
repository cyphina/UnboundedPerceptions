// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "Minimap.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UMinimap : public UMyUserWidget
{
   GENERATED_BODY()

 public:
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
   void ToggleDirectionArrowVisibility(bool shouldShow); // Toggle visibility, true means visible

   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
   void UpdateDirectionArrow(float arrowAngle); // set the direction arrow to some new render angle (in degrees please!)
};
