// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/Item.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "Inventory.generated.h"

class UBackpack;
class ABaseHero;
class UInventoryView;

/**
 * @brief Inventory C++ base class. Should be used UI for any item management system like character inventory, treasure chests, etc.
 */
UCLASS(Blueprintable)
class MYPROJECT_API UInventory : public UMyDraggableWidget
{
   GENERATED_BODY()

public:
   /**
    * Runs when an itemSlot in the inventoryView is clicked on.  Depending on the inventory type, different things may occur.
    * @iSlot - Index of the slot in the backpack to be used
    */
   UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
   void UseItem(int32 iSlot);

   /** Used to update the view whenever change occurs within the backpack corresponding to our inventory
    * TODO: Try and only have to reload items that changed.
    */
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory Functions")
   void LoadItems();

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Functions")
   UBackpack* GetBackpack() const { return backpack; }

   UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
   void SetBackPack(UBackpack* bPack) { backpack = bPack; }

   FMyItem GetBackpackItemAtSlot(int slotIndex) const;

protected:
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Functions")
   void UseItemAtInventorySlot(int32 iSlot);
   virtual void UseItemAtInventorySlot_Implementation(int32 iSlot) PURE_VIRTUAL(UInventory::UseItemAtInventorySlot,);

   void NativeConstruct() override;
   bool OnWidgetAddToViewport_Implementation() override;

   UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
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
