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

<<<<<<< HEAD
=======
DECLARE_EVENT_OneParam(UInventory, FOnInventoryItemSelected, int);

>>>>>>> componentrefactor
/**
 * @brief Inventory C++ base class. Should be used UI for any item management system like character inventory, treasure chests, etc.
 */
UCLASS(Blueprintable)
class MYPROJECT_API UInventory : public USlotContainer
{
   GENERATED_BODY()

 public:
   /**
<<<<<<< HEAD
    * Runs when an itemSlot in the inventoryView is clicked on.  Depending on the inventory type, different things may occur.
    * @iSlot - Index of the slot in the backpack (not inventory view slot index) to be used
    */
   UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Functions")
   void         UseItemAtInventorySlot(int32 iSlot);
   virtual void UseItemAtInventorySlot_Implementation(int32 iSlot) PURE_VIRTUAL(UInventory::UseItemAtInventorySlot, );

   /** Used to update the view whenever change occurs within the backpack corresponding to our inventory */
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory Functions")
   void LoadItems();

=======
    * Used to update the view whenever change occurs within the backpack corresponding to our inventory
    * Has to update all the slots at once so it's better to reload only the slots that are changed. Also event driven architecture
    * could help if we one day learn to implement networking...
    */
   UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
   void LoadItems();

   UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
   void ReloadSlots(TSet<int> slotIndices);

>>>>>>> componentrefactor
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Functions")
   UBackpack* GetBackpack() const { return backpack; }

   UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
   void SetBackPack(UBackpack* bPack) { backpack = bPack; }

<<<<<<< HEAD
   UInventoryView* GetInventoryView() const { return inventoryView; }
   
 protected:
   void NativeConstruct() override;
   bool OnWidgetAddToViewport_Implementation() override;

=======
   /** Maps slot index in the list of displayed widget to the actual index in the backpack (they can be different due to things like pagination) */
   int GetCorrespondingBackpackIndex(int slotIndex) const;

   FMyItem GetBackpackItemAtSlot(int slotIndex) const;

   FOnInventoryItemSelected& OnInventoryItemSelected() { return OnItemSelectedEvent; }

   int GetNumValidItems() const override { return GetInventorySlots().Num(); }
   
 protected:
   void NativeOnInitialized() override;
   bool OnWidgetAddToViewport_Implementation() override;

   TArray<UActionSlot*>& GetInventorySlots() const;

>>>>>>> componentrefactor
   UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
   UInventoryView* inventoryView;

 private:
   /** Updates a slot's count and image */
   void UpdateSlot(int slotIndex);

   void ResetSlot(UActionSlot* actionSlot);
	
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
