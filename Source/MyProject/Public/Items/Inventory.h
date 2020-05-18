// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/Item.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "Inventory.generated.h"

class UBackpack;
class ABaseHero;
class UInventoryView;

/*Inventory C++ base class.  Widgets don't get constructors.  Can be used as UI for chacter inventory and for treasure chest UI and such.
 *All Inventory does is display items in a backpack and lets us use them.*/

UCLASS(Blueprintable)
class MYPROJECT_API UInventory : public UMyDraggableWidget
{
   GENERATED_BODY()

 public:
   void NativeConstruct() override;

   bool OnWidgetAddToViewport_Implementation() override;

   /**Runs when an itemSlot in the inventoryView is clicked on.  Depending on the inventory type, different things may occur.
    * @iSlot - Index of the slot in the backpack (not inventory view slot index) to be used
    */
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Functions")
   void         UseItemAtInventorySlot(int32 iSlot);
   virtual void UseItemAtInventorySlot_Implementation(int32 iSlot) PURE_VIRTUAL(UInventory::UseItemAtInventorySlot, );

   /**Used to update the view whenever change occurs within our inventory*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory Functions")
   void LoadItems();

   /**Accessors for backpack*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Functions")
   UBackpack* GetBackpack() const { return backpack; }

   /**Accessors for backpack*/
   UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
   void SetBackPack(UBackpack* bPack) { backpack = bPack; }

   /**a reference to our main inventory UI widget*/
   UPROPERTY(BlueprintReadWrite)
   UInventoryView* inventoryView;

 private:
   /**backpack reference for whatever inventory we are displaying*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UIInitialParams", Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
   UBackpack* backpack;

   /**Used to easily set default rows and columns when creating inventory widget*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UIInitialParams", Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
   int rows;

   UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UIInitialParams", Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
   int columns;
};
