// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTSBrowserWidget.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTSBrowserWidget : public UInterface
{
   GENERATED_BODY()
};

class UBluEye;

/**
 * Base class for any browser widgets that can be interacted with and need setup
 */
class MYPROJECT_API IRTSBrowserWidget
{
   GENERATED_BODY()

 public:
   /** Setups this browser widget. Create the browser in NativeConstruct first, then setup the texture and configures initial browser parameters*/
   UFUNCTION()
   virtual void StartDisplay(int browserWidth, int browserHeight) = 0;

   /** Updates the browser with new information used in the reactive apps the browser is displayed (svelte apps)*/
   virtual void UpdateInformation(const FString& updateFunctionName, FStringView updateJSON) = 0;
};
