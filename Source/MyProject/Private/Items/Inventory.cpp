#include "MyProject.h"
#include "Inventory.h"
#include "ActionSlot.h"
#include "Backpack.h"
#include "ForEach.h"
#include "InventoryView.h"
#include "ItemDelegateContext.h"
#include "ItemManager.h"

void UInventory::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   inventoryView->inventoryRef = this;
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemsTransferred().AddUObject(this, &UInventory::OnItemsTransferred);
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemsSwapped().AddUObject(this, &UInventory::OnItemsSwapped);
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

TArray<UActionSlot*>& UInventory::GetInventorySlots() const
{
   return inventoryView->inventorySlots;
}

void UInventory::LoadItems()
{
   if(GetBackpack())
   {
      Algo::ForEach(GetInventorySlots(), [this](UActionSlot* actionSlot) {
         ResetSlot(actionSlot);
      });

      const int maxUpdateIndex = FMath::Min(GetInventorySlots().Num(), GetBackpack()->GetItemMax());
      for(int i = 0; i < maxUpdateIndex; ++i)
      {
         UpdateSlot(i);
      }
   }
}

void UInventory::ReloadSlots(TSet<int> slotIndices)
{
   if(backpack)
   {
      const int maxUpdateIndex = FMath::Min(GetInventorySlots().Num(), GetBackpack()->GetItemMax());
      for(int slotIndex : slotIndices)
      {
         if(slotIndex >= 0 && slotIndex < maxUpdateIndex)
         {
            UpdateSlot(slotIndex);
         }
      }
   }
}

void UInventory::UpdateSlot(int slotIndex)
{
   UActionSlot* actionSlot = GetInventorySlots()[slotIndex];
   if(const FMyItem item = GetBackpackItemAtSlot(slotIndex))
   {
      FItemLookupRow* itemInfo = UItemManager::Get().GetItemInfo(item.id);
      actionSlot->SetSlotImage(itemInfo->image);
      if(itemInfo->isStackable)
      {
         actionSlot->SetInfo(FText::AsNumber(item.count));
      }
   }
   else
   {
      ResetSlot(actionSlot);
   }
}

void UInventory::ResetSlot(UActionSlot* actionSlot)
{
   actionSlot->SetSlotImage(nullptr);
   actionSlot->SetInfo(FText::GetEmpty());
}

void UInventory::OnItemsTransferred(const UBackpack& originalBackpack, const UBackpack& newBackpack, const FBackpackUpdateResult& removeResult,
                                    const FBackpackUpdateResult& addResult)
{
   if(GetBackpack() == &originalBackpack)
   {
      ReloadSlots(TSet<int>(removeResult.updatedBackpackIndices));
   }

   if(GetBackpack() == &newBackpack)
   {
      ReloadSlots(TSet<int>(addResult.updatedBackpackIndices));
   }
}

void UInventory::OnItemsSwapped(const UBackpack& pack1, const UBackpack& pack2, int slot1, int slot2)
{
   TSet<int> updatedSlots;
   if(&pack1 == GetBackpack())
   {
      updatedSlots.Add(slot1);
   }
   if(&pack2 == GetBackpack())
   {
      updatedSlots.Add(slot2);
   }

   ReloadSlots(updatedSlots);
}

int UInventory::GetCorrespondingBackpackIndex(int slotIndex) const
{
   return inventoryView->GetCorrespondingBackpackIndex(slotIndex);
}

FMyItem UInventory::GetBackpackItemAtSlot(int slotIndex) const
{
   const float correspondingSlotIndex = inventoryView->GetCorrespondingBackpackIndex(slotIndex);
   if(correspondingSlotIndex >= 0 && correspondingSlotIndex < GetBackpack()->GetItemMax())
   {
      return GetBackpack()->GetItem(correspondingSlotIndex);
   }
   return FMyItem();
}
