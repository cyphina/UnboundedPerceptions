#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidgetExtensions/MyUserWidget.h"
#include "InventoryView.generated.h"

class UActionSlot;
class UInventory;

/**
* An inventory view is a way to display the inventories contents.  Examples include paged inventory and scrollbar inventory views.
*/
UCLASS()
class MYPROJECT_API UInventoryView : public UMyUserWidget
{
   GENERATED_BODY()

public:
   /** Way to return to the default view. For a paged inventory view this returns to page 1, for a scroll inventory view this scrolls to the top. */
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "InventoryViewFunctions")
   void Reset();

   /** Mapping of visible inventory slots to items in backpack. Useful for the paged inventory where we have only 30 slots displayed at a time but actually like 100 backpack items */
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, BlueprintPure, Category = "InventoryViewFunctions")
   int GetCorrespondingBackpackIndex(int slotNum);

   /**Reference to the container that lists the items we have.  The slots need this.*/
   UPROPERTY(BlueprintReadWrite, Category = "References")
   UInventory* inventoryRef;

   UPROPERTY(BlueprintReadWrite)
   TArray<UActionSlot*> inventorySlots;
};
