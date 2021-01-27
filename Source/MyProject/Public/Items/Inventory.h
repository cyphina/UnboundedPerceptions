// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlotContainer.h"
#include "Items/Item.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "Inventory.generated.h"

struct FBackpackUpdateResult;
class UActionSlot;
class UBackpack;
class ABaseHero;
class UInventoryView;

DECLARE_EVENT_OneParam(UInventory, FOnInventoryItemSelected, int);

/**
 * @brief Inventory C++ base class. Should be used UI for any item management system like character inventory, treasure chests, etc.
 */
UCLASS(Blueprintable)
class MYPROJECT_API UInventory : public USlotContainer
{
   GENERATED_BODY()

 public:
   /**
    * Used to update the view whenever change occurs within the backpack corresponding to our inventory
    */
   UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
   void LoadItems();

   UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
   void ReloadSlots(TSet<int> slotIndices);

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Functions")
   UBackpack* GetBackpack() const { return backpack; }

   UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
   void SetBackPack(UBackpack* bPack) { backpack = bPack; }

   /** Maps slot index in the list of displayed widget to the actual index in the backpack (they can be different due to things like pagination) */
   int GetCorrespondingBackpackIndex(int slotIndex) const;

   FMyItem GetBackpackItemAtSlot(int slotIndex) const;

   FOnInventoryItemSelected& OnInventoryItemSelected() { return OnItemSelectedEvent; }

 protected:
   void NativeOnInitialized() override;
   bool OnWidgetAddToViewport_Implementation() override;

   TArray<UActionSlot*>& GetInventorySlots() const;

   UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
   UInventoryView* inventoryView;

   UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
   TSubclassOf<UActionSlot> slotClass;

   UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
   UTexture2D* defaultSlotTexture;

 private:
   /** Updates a slot's count and image */
   void UpdateSlot(int slotIndex);

   void OnItemsTransferred(const UBackpack& originalBackpack, const UBackpack& newBackpack, const FBackpackUpdateResult& removeResult,
                           const FBackpackUpdateResult& addResult);
   void OnItemsSwapped(const UBackpack& pack1, const UBackpack& pack2, int slot1, int slot2);

   /**backpack reference for whatever inventory we are displaying*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UIInitialParams", Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
   UBackpack* backpack;

   /**Used to easily set default rows and columns when creating inventory widget*/
   UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UIInitialParams", Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
   int rows;

   UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UIInitialParams", Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
   int columns;

   FOnInventoryItemSelected OnItemSelectedEvent;
};
