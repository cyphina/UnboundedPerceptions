#include "MyProject.h"
#include "Inventory.h"

#include "ActionSlot.h"
#include "Backpack.h"
#include "BaseHero.h"
#include "ForEach.h"
#include "InventoryView.h"
#include "ItemManager.h"

void UInventory::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   inventoryView->inventoryRef = this;
}

bool UInventory::OnWidgetAddToViewport_Implementation()
{
   Super::OnWidgetAddToViewport_Implementation();

   if(!backpack)
   {
      return false;
   }
   
   LoadItems();
   return true;
}

void UInventory::UseItem(int32 iSlot)
{
   if(!GetBackpack()->IsEmptySlot(iSlot))
   {
      UseItemAtInventorySlot(inventoryView->GetCorrespondingBackpackIndex(iSlot));
      LoadItems();
   }
}

void UInventory::LoadItems()
{
   if(GetBackpack())
   {
      auto resetSlot = [this](UActionSlot* actionSlot)
      {
         actionSlot->SetSlotImage(defaultSlotTexture);
         actionSlot->SetInfo(FText::GetEmpty());
      };

      Algo::ForEach(inventorySlots, resetSlot);

      for(int i = 0; i < inventorySlots.Num(); ++i)
      {
         UpdateSlot(i);
      }
   }
}

void UInventory::ReloadSlots(TSet<int> slotIndices)
{
   for(int slotIndex : slotIndices)
   {
      UpdateSlot(slotIndex);
   }
}

void UInventory::UpdateSlot(int slotIndex)
{
   UActionSlot* actionSlot = inventorySlots[slotIndex];
   if(const FMyItem item = GetBackpackItemAtSlot(slotIndex))
   {
      FItemLookupRow* itemInfo = UItemManager::Get().GetItemInfo(item.id);
      actionSlot->SetSlotImage(itemInfo->image);
      if(itemInfo->isStackable)
      {
         actionSlot->SetInfo(FText::AsNumber(item.count));
      }
   }
}

FMyItem UInventory::GetBackpackItemAtSlot(int slotIndex) const
{
   return GetBackpack()->GetItem(inventoryView->GetCorrespondingBackpackIndex(slotIndex));
}
