#include "MyProject.h"
#include "Backpack.h"
#include "ItemManager.h"
#include "LevelSaveStructs.h"
#include "BaseHero.h"
#include "ItemDelegateContext.h"
#include "JsonTypes.h"
#include "RTSIngameWidget.h"

UBackpack::UBackpack()
{
   items.Reserve(INIT_CAPACITY);
}

UBackpack* UBackpack::CreateBackpack(UObject* outer, const int itemMax)
{
   UBackpack* backpack = NewObject<UBackpack>(outer);
   backpack->SetItemMax(itemMax);
   return backpack;
}

FBackpackUpdateResult UBackpack::AddItem(FMyItem newItem)
{
   check(newItem.id > 0);

   const bool bIsNewItemStackable = UItemManager::Get().GetItemInfo(newItem.id)->isStackable;
   if(bIsNewItemStackable)
   {
      return AddStackableItem(newItem);
   }
   return AddUnstackableItem(newItem);
}

FBackpackUpdateResult UBackpack::AddStackableItem(FMyItem itemToAdd)
{
   int                   index = 0;
   TArray<int>           updatedSlots;
   FBackpackUpdateResult updateResultData{itemToAdd};

   // Look through slots that aren't empty for existing stacks
   for(FMyItem& i : items)
   {
      if(i.id == itemToAdd.id && i.count != STACK_MAX)
      {
         // If this item slot's stack is not filled up by our new additions
         if(i.count + itemToAdd.count <= STACK_MAX)
         {
            i.count += itemToAdd.count;
            itemToAdd.count = 0;
            return updateResultData({index}, true, itemToAdd.count);
         }

         // Else fill up the slot and find another one to try and fill
         itemToAdd.count -= STACK_MAX - i.count;
         i.count = STACK_MAX;
         updatedSlots.Add(index);
      }
      ++index;
   }

   // We filled up existing slots, so now we check for empty slots
   while(itemToAdd.count > 0)
   {
      if(items.Num() >= backpack_max_items) // If there are extra slots in the backpack
      {
         return updateResultData(updatedSlots, false, itemToAdd.count);
      }

      if(const int emptySlotIndex = FindEmptySlot(); emptySlotIndex != -1)
      {
         const int howManyItemsAdded = FMath::Min(STACK_MAX, itemToAdd.count);
         items.Insert(emptySlotIndex, FMyItem(itemToAdd.id, howManyItemsAdded));
         itemToAdd.count -= howManyItemsAdded;
         updatedSlots.Add(emptySlotIndex);
      }
   }

   return updateResultData(updatedSlots, true, itemToAdd.count);
}

FBackpackUpdateResult UBackpack::AddUnstackableItem(FMyItem itemToAdd)
{
   if(items.Num() < backpack_max_items)
   {
      TArray<int>           updatedSlots;
      FBackpackUpdateResult updateResultData{itemToAdd};

      while(itemToAdd.count > 0)
      {
         if(const int emptySlotIndex = FindEmptySlot(); emptySlotIndex != -1)
         {
            items.Insert(emptySlotIndex, FMyItem(itemToAdd.id, 1));
            itemToAdd.count -= 1;
            updatedSlots.Add(emptySlotIndex);
         }
         else
         {
            return updateResultData(updatedSlots, false, itemToAdd.count);
         }
      }
      return updateResultData(updatedSlots, true, itemToAdd.count);
   }

   return FBackpackUpdateResult(itemToAdd.count);
}

FBackpackUpdateResult UBackpack::AddItemToSlot(FMyItem newItem, int slot)
{
   check(newItem.id > 0);
   check(newItem.count <= STACK_MAX);
   check(IsItemStackable(newItem.id) || newItem.count == 1); // Ensure this item is stackable or it is just a single item
   check(slot >= 0 && slot <= backpack_max_items);

   FBackpackUpdateResult updateResultData{newItem};

   // Check if this slot has something in it already
   const bool isSlotAllocated = items.IsAllocated(slot);

   if(isSlotAllocated)
   {
      if(items[slot].id == newItem.id)
      {
         if(IsItemStackable(newItem.id))
         {
            // if we have an item in our slot already
            {
               if(items[slot].count + newItem.count < STACK_MAX) // If we have enough room in our stack to add these items
               {
                  items[slot].count += newItem.count;
                  return updateResultData({slot}, true, newItem.count);
               } // if we don't have enough room we add the item to a full slot, and the rest of the item is still in our cursor
               newItem.count -= STACK_MAX - items[slot].count;
               items[slot].count = STACK_MAX;
               return updateResultData({slot}, false, newItem.count);
            }
         }
      }
      return updateResultData;
   }

   // Empty slot so just insert whatever logical item we have
   items.Insert(slot, FMyItem(newItem.id, newItem.count));
   return updateResultData({slot}, true, newItem.count);
}

TArray<FBackpackUpdateResult> UBackpack::AddItems(const TArray<FMyItem>& newItems)
{
   TArray<FBackpackUpdateResult> updateResults;
   // Don't need to check for size of our backpack since newItems may contain stackable items
   for(const FMyItem& newIt : newItems)
   {
      FBackpackUpdateResult updateResult = AddItem(newIt);
      updateResults.Add(updateResult);
      if(!updateResult.bSuccessfulOperation)
      {
         break;
      }
   }
   return updateResults;
}

FBackpackUpdateResult UBackpack::RemoveItem(FMyItem itemToRemove)
{
   const int newID       = itemToRemove.id;
   const int removeCount = itemToRemove.count;

   check(itemToRemove.id > 0);

   if(items.Num() > 0)
   {
      TArray<int>           updatedSlots;
      FBackpackUpdateResult updateResultData{itemToRemove};

      while(itemToRemove.count > 0)
      {
         const int index = FindItem(newID);
         if(index == INDEX_NONE)
         {
            return updateResultData(updatedSlots, false, itemToRemove.count);
         }

         if(IsItemStackable(itemToRemove.id))
         {
            if(items[index].count > removeCount)
            {
               items[index].count -= removeCount;
            }
            else
            {
               EmptySlot(index);
               items[index].count -= 1;
            }
         }
         updatedSlots.Add(index);
      }
      return updateResultData(updatedSlots, true, 0);
   }

   FBackpackUpdateResult updateResultData(itemToRemove);
   updateResultData.numUpdatedItemsRemaining = itemToRemove.count;
   return updateResultData;
}

FBackpackUpdateResult UBackpack::RemoveItemAtSlot(const int slot, const int removeCount)
{
   if(items.Num() > 0)
   {
      if(items.IsAllocated(slot))
      {
         FBackpackUpdateResult updateResultData({slot}, true, items[slot].id, removeCount, 0);
         if(UItemManager::Get().GetItemInfo(items[slot].id)->isStackable && items[slot].count > removeCount)
         {
            updateResultData.numUpdatedItemsRemaining = removeCount - items[slot].count;
            items[slot].count                         = FMath::Min(removeCount, items[slot].count - removeCount);
         }
         else
         {
            updateResultData.numUpdatedItemsRemaining = removeCount - 1;
            items.RemoveAt(slot);
         }
         return updateResultData;
      }
   }
   return FBackpackUpdateResult(removeCount);
}

TArray<FBackpackUpdateResult> UBackpack::RemoveItems(const TArray<FMyItem>& itemsToRemove)
{
   TArray<FBackpackUpdateResult> updateResults;
   for(const FMyItem& newIt : itemsToRemove)
   {
      FBackpackUpdateResult updateResultData = RemoveItem(newIt);
      updateResults.Emplace(updateResultData);
      if(!updateResultData.bSuccessfulOperation)
      {
         break;
      }
   }
   return updateResults;
}

FBackpackUpdateResult UBackpack::EmptySlot(const int slot)
{
   FBackpackUpdateResult updateResultInfo{{slot}, true, items[slot].id, items[slot].count, 0};
   items.RemoveAtUninitialized(slot);
   return updateResultInfo;
}

void UBackpack::EmptyAll()
{
   items.Empty(backpack_max_items);
   items.Reserve(backpack_max_items);
}

TPair<FBackpackUpdateResult, FBackpackUpdateResult> UBackpack::TransferItems(UBackpack* otherPack, int transferSlot)
{
   if(otherPack->items.IsAllocated(transferSlot) && otherPack->items.Num() < backpack_max_items)
   {
      const FMyItem& otherItem = otherPack->items[transferSlot];
      if(otherItem.id > 0 && otherItem.count > 0)
      {
         const FBackpackUpdateResult addItemResult = AddItem(otherItem);
         FBackpackUpdateResult       removeItemFromOtherPackResult;
         if(addItemResult.bSuccessfulOperation) // Check if successfully transferred everything
         {
            removeItemFromOtherPackResult = otherPack->EmptySlot(transferSlot);
         }
         else
         {
            removeItemFromOtherPackResult = otherPack->RemoveItemAtSlot(transferSlot, addItemResult.numUpdatedItemsRequested - addItemResult.numUpdatedItemsRemaining);
         }
         return TPair<FBackpackUpdateResult, FBackpackUpdateResult>{removeItemFromOtherPackResult, addItemResult};
      }
      UE_LOG(LogTemp, Error, TEXT("When transfering items, found an item that has an invalid id or count"));
      return TPair<FBackpackUpdateResult, FBackpackUpdateResult>();
   }
   return TPair<FBackpackUpdateResult, FBackpackUpdateResult>();
}

void UBackpack::SwapItems(UBackpack* pack1, UBackpack* pack2, const int slot1, const int slot2)
{
   if(pack2->items.IsAllocated(slot2))
   {
      if(pack1->items.IsAllocated(slot1)) // if there's an item in the slot we want to swap
      {
         const FMyItem itemFromOtherPack = pack2->items[slot2];
         pack2->items[slot2]             = pack1->items[slot1];
         pack1->items[slot1]             = itemFromOtherPack;
      }
      else // if not, just insert an item there
      {
         pack1->items.Insert(slot1, pack2->items[slot2]);
         pack2->EmptySlot(slot2);
      }
   }
   else
   {
      if(pack1->items.IsAllocated(slot1))
      {
         pack2->items.Insert(slot2, pack1->items[slot1]);
         pack1->EmptySlot(slot1);
      }
   }
}

FMyItem UBackpack::GetItem(const int slot) const
{
   if(items.IsAllocated(slot))
   {
      return items[slot];
   }
   return FMyItem();
}

int UBackpack::Count() const
{
   return items.Num();
}

TArray<FMyItem> UBackpack::GetItems()
{
   TArray<FMyItem> itemsToGet;
   for(auto it = items.CreateIterator(); it; ++it)
   {
      itemsToGet.Add(*it);
   }
   return itemsToGet;
}

TArray<int> UBackpack::GetItemIndices()
{
   TArray<int> indicesToGet;
   for(auto it = items.CreateIterator(); it; ++it)
   {
      indicesToGet.Add(it.GetIndex());
   }
   return indicesToGet;
}

int UBackpack::FindEmptySlot() const
{
   for(int i = 0; i < items.GetMaxIndex(); i++)
   {
      if(!items.IsAllocated(i))
      {
         return i;
      }
   }
   return -1;
}

bool UBackpack::IsEmptySlot(const int slotIndex) const
{
   check(static_cast<unsigned>(slotIndex) < static_cast<unsigned>(GetItemMax())) return !items.IsAllocated(slotIndex) ? true : false;
}

void UBackpack::SetItemMax(const int newMax)
{
   check(newMax >= backpack_max_items);
   backpack_max_items = newMax;
   items.Reserve(backpack_max_items);
}

int UBackpack::FindItem(const int itemID) const
{
   for(auto it = items.CreateConstIterator(); it; ++it)
   {
      if((*it).id == itemID) return it.GetIndex();
   }
   return INDEX_NONE;
}

int UBackpack::FindItemCount(int itemID) const
{
   int itemCount = 0;
   for(auto it = items.CreateConstIterator(); it; ++it)
   {
      if((*it).id == itemID) itemCount += (*it).count;
   }
   return itemCount;
}

void UBackpack::SaveBackpack(FBackpackSaveInfo& backpackInfo)
{
   TArray<FMyItem> itemsToSave = GetItems();
   TArray<int>     itemIndices = GetItemIndices();

   for(FMyItem item : itemsToSave)
   {
      backpackInfo.itemIDs.Add(item.id);
      backpackInfo.itemCounts.Add(item.count);
   }

   for(int itemSlot : itemIndices)
   {
      backpackInfo.itemSlots.Add(itemSlot);
   }
}

void UBackpack::LoadBackpack(FBackpackSaveInfo& backpackInfo)
{
   for(int i = 0; i < backpackInfo.itemIDs.Num(); ++i)
   {
      FMyItem item{backpackInfo.itemIDs[i], backpackInfo.itemCounts[i]};
      AddItemToSlot(item, backpackInfo.itemSlots[i]);
   }
}

bool UBackpack::IsItemStackable(int itemID)
{
   return UItemManager::Get().GetItemInfo(itemID)->isStackable;
}

void UBackpack::BeginDestroy()
{
   Super::BeginDestroy();
}
