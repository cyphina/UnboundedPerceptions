// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "LoadingWidget.generated.h"

/**
 * Widget for loading screen
 */
UCLASS()
class MYPROJECT_API ULoadingWidget : public UMyUserWidget
{
   GENERATED_BODY()

   ULoadingWidget();

   /**List of possible loading screen tips*/
   TArray<FText> tipList;

 public:
   void NativeConstruct() override;

   /**Has the level finished loading?*/
   UPROPERTY(BlueprintReadWrite, Category = "Data")
   bool isReadyToContinue;

   /**Current loading screen tip that is displayed*/
   UPROPERTY(BlueprintReadWrite, Category = "Data")
   FText tip;

   /**Callback when level finished precaching*/

   /**Display a message when loading is finished*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Callbacks")
   void OnLoadingFinished();

   /**Show a random game tip*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Data")
   void SetTip(const FText& newTip);
};
