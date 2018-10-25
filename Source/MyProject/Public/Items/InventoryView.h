// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "InventoryView.generated.h"

/**
 * An inventory view is a way to display the inventories contents.  Examples include paged inventory and scrollbar inventory views.
 */

class UInventory;

UCLASS()
class MYPROJECT_API UInventoryView : public UMyUserWidget
{
   GENERATED_BODY()

 public:
   /**Reference to the container that lists the items we have.  The slots need this.*/
   UPROPERTY(BlueprintReadWrite, Category = "References", Meta = (ExposeOnSpawn = true))
   UInventory* inventoryRef;

   /**Way to return to the default view*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InventoryViewFunctions")
   void Reset();

   /**Mapping of visible inventory slots to items in backpack*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintPure, Category = "InventoryViewFunctions")
   int GetCorrespondingBackpackIndex(int slotNum);

   /**Way to load items from backpack to visible inventory slots*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory Functions")
   void LoadItems();
};
