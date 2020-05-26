// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Backpack.h"
#include "ItemManager.h"
#include "LevelSaveStructs.h"

UBackpack::UBackpack()
{
   items = TSparseArray<FMyItem>();
   items.Reserve(INIT_CAPACITY);
}

bool UBackpack::AddItem(FMyItem& newItem)
{
#if UE_EDITOR
   check(newItem.id > 0);
#endif

   const bool bIsNewItemStackable = UItemManager::Get().GetItemInfo(newItem.id)->isStackable;
   if(bIsNewItemStackable) // If this is a stackable item
   {
      for(FMyItem& i : items) // loop through items and see if there's one already inside
      {
         if(i.id == newItem.id && i.count != STACK_MAX) // If there's a matching slot with this type of item and it's not already full
         {
            if(i.count + newItem.count <= STACK_MAX) // If the slot has enough room to add these additional items
            {
               i.count += newItem.count; // Add the count of the newitems to our original item count
               newItem.count = 0;
               return true;
            } else // If this slot doesn't have enough room for all the additional item amount we're trying to add, add as many as we can to this slot and then look for other ones
            {
               newItem.count -= STACK_MAX - i.count; // Lower the item count since we added some items
               i.count = STACK_MAX;                  // Fill up the slot
            }
         }
      }

      while(newItem.count > 0) {
         if(items.Num() >= backpack_max_items) // If there are extra slots in the backpack
            return false;

         if(const int emptySlotIndex = FindEmptySlot(); emptySlotIndex != -1) { // KEep adding stacsk to the new slot until we fill up the backpack or run down the count
            int howManyItemsAdded = FMath::Min(STACK_MAX, newItem.count);
            items.Insert(emptySlotIndex, FMyItem(newItem.id, howManyItemsAdded));
            newItem.count -= howManyItemsAdded;
         }
      }
      return true;
   } else {                     // If item is not stackable
      if(items.Num() < backpack_max_items) // If there are extra slots in the backpack
      {
         // Add each unstackable item to a new slot
         while(newItem.count > 0) {
            if(const int emptySlotIndex = FindEmptySlot(); emptySlotIndex != -1) {
               items.Insert(emptySlotIndex, FMyItem(newItem.id, 1));
               newItem.count -= 1;
            } else
               return false;
         }
         return true;
      }
      // Not enough space to add all the unstackable items
      return false;
   }
}

bool UBackpack::AddItemToSlot(FMyItem& newItem, int slot)
{
   const bool bIsNewItemStackable = UItemManager::Get().GetItemInfo(newItem.id)->isStackable;

#if UE_EDITOR
   check(newItem.id > 0);
   check(newItem.count <= STACK_MAX);
   check(bIsNewItemStackable || newItem.count == 1) // Ensure this item is stackable or is just a single item
       check(slot >= 0 && slot <= backpack_max_items);
#endif

   // Check if this slot has something in it already
   const bool isSlotAllocated = items.IsAllocated(slot);

   if(isSlotAllocated) {
      if(items[slot].id == newItem.id) {
         if(bIsNewItemStackable) { // if we have an item in our slot already
            {
               if(items[slot].count + newItem.count < STACK_MAX) // if we have enough room in our stack to add these items
               {
                  items[slot].count += newItem.count;
                  return true;
               } else // if we don't have enough room we add the item to a full slot, and the rest of the item is still in our cursor
               {
                  newItem.count -= STACK_MAX - items[slot].count;
                  items[slot].count = STACK_MAX;
                  return false;
               }
            }
         }
      }
      return false;
   } else {
      // New slot just insert whatever logical item we have
      items.Insert(slot, FMyItem(newItem.id, newItem.count));
      return true;
   }
}

bool UBackpack::AddItems(TArray<FMyItem>& newItems)
{
   // Don't need to check for size of our backpack since newItems may contain stackable items
   for(FMyItem& newIt : newItems) {
      if(!AddItem(newIt))
         return false;
   }
   return true;
}

bool UBackpack::RemoveItem(const FMyItem& itemToRemove)
{
   int newID    = itemToRemove.id;
   int newCount = itemToRemove.count;

#if UE_EDITOR
   check(newID > 0);
#endif

   if(items.Num() > 0) {
      int index = FindItem(newID);

      if(index == -1)
         return false;

      if(items[index].count > newCount) {
         items[index].count -= newCount;
         return true;
      } else
         EmptySlot(index);
   }
   return false;
}

bool UBackpack::RemoveItemAtSlot(int slot, int removeCount)
{
   if(items.Num() > 0) {
      if(items.IsAllocated(slot)) {
         if(UItemManager::Get().GetItemInfo(items[slot].id)->isStackable && items[slot].count > removeCount) {
            items[slot].count -= removeCount;
            if(items[slot].count < 0)
               items.RemoveAt(slot);
         } else {
            items.RemoveAt(slot);
         }
         return true;
      }
   }
   return false;
}

bool UBackpack::RemoveItems(const TArray<FMyItem>& itemsToRemove)
{
   bool success = true;
   for(const FMyItem& newIt : itemsToRemove) {
      if(!RemoveItem(newIt))
         success = false;
   }
   return success;
}

void UBackpack::EmptySlot(int slot)
{
   items.RemoveAtUninitialized(slot);
}

void UBackpack::EmptyAll()
{
   items.Empty(backpack_max_items);
   items.Reserve(backpack_max_items);
}

bool UBackpack::TransferItems(UBackpack* otherPack, int transferSlot)
{
   if(otherPack->items.IsAllocated(transferSlot) && otherPack->items.Num() < backpack_max_items) {
      FMyItem* otherItem = &otherPack->items[transferSlot];

      if(otherItem && otherItem->id > 0) {
         const int initalItemCount = otherItem->count;
         if(AddItem(*otherItem)) // Check if successfully transferred everything
         {
            otherPack->EmptySlot(transferSlot);
            return true;
         } else // We have leftovers
         {
            otherPack->RemoveItemAtSlot(transferSlot, initalItemCount - otherItem->count);
         }
      }
   }
   return false;
}

void UBackpack::SwapItems(UBackpack* otherPack, int slot1, int slot2)
{
   if(otherPack->items.IsAllocated(slot2)) {
      if(items.IsAllocated(slot1)) // if there's an item in the slot we want to swap
      {
         FMyItem itemFromOtherPack = otherPack->items[slot2];
         otherPack->items[slot2]   = items[slot1];
         items[slot1]              = itemFromOtherPack;
      } else // if not, just insert an item there
      {
         items.Insert(slot1, otherPack->items[slot2]);
         otherPack->EmptySlot(slot2);
      }
   }
}

FMyItem UBackpack::GetItem(int slot) const
{
   check(slot >= 0 && slot < backpack_max_items);
   if(items.IsAllocated(slot)) {
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
   for(auto it = items.CreateIterator(); it; ++it) {
      itemsToGet.Add(*it);
   }
   return itemsToGet;
}

TArray<int> UBackpack::GetItemIndices()
{
   TArray<int> indicesToGet;
   for(auto it = items.CreateIterator(); it; ++it) {
      indicesToGet.Add(it.GetIndex());
   }
   return indicesToGet;
}

int UBackpack::FindEmptySlot() const
{
   for(int i = 0; i < items.GetMaxIndex(); i++) {
      if(!items.IsAllocated(i)) {
         return i;
      }
   }
   return -1;
}

bool UBackpack::IsEmptySlot(int slotIndex) const
{
   check((unsigned)slotIndex < (unsigned)GetItemMax())
   return !items.IsAllocated(slotIndex) ? true : false;
}

void UBackpack::SetItemMax(int newMax)
{
   check(newMax >= backpack_max_items);
   backpack_max_items = newMax;
   items.Reserve(backpack_max_items);
}

int UBackpack::FindItem(int itemID)
{
   for(auto it = items.CreateIterator(); it; ++it) {
      if((*it).id == itemID)
         return it.GetIndex();
   }
   return -1;
}

int UBackpack::FindItemCount(int itemID)
{
   int itemCount = 0;
   for(auto it = items.CreateIterator(); it; ++it) {
      if((*it).id == itemID)
         itemCount += (*it).count;
   }
   return itemCount;
}

void UBackpack::SaveBackpack(FBackpackSaveInfo& backpackInfo)
{
   TArray<FMyItem> itemsToSave = GetItems();
   TArray<int>     itemIndices = GetItemIndices();

   for(FMyItem item : itemsToSave) {
      backpackInfo.itemIDs.Add(item.id);
      backpackInfo.itemCounts.Add(item.count);
   }

   for(int itemSlot : itemIndices) {
      backpackInfo.itemSlots.Add(itemSlot);
   }
}

void UBackpack::LoadBackpack(FBackpackSaveInfo& backpackInfo)
{
   for(int i = 0; i < backpackInfo.itemIDs.Num(); ++i) {
      FMyItem item{backpackInfo.itemIDs[i], backpackInfo.itemCounts[i]};
      AddItemToSlot(item, backpackInfo.itemSlots[i]);
   }
}
