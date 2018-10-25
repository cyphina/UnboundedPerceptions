// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CollectiblesUI.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API UCollectiblesUI : public UUserWidget
{
   GENERATED_BODY()

 protected:
   /*Responsible for adding the ItemToAdd to the UI*/
   UFUNCTION(BlueprintImplementableEvent, Category = UI)
   void AddItemToUI();

   /*Next item that is going to get added to UI*/
   UPROPERTY(BlueprintReadOnly)
   FText ItemToAdd;

 public:
   /*Adds name of the item we collected inside the UI by using AddItemToUI*/

   void AddItemToUI(FText ItemName);
};
