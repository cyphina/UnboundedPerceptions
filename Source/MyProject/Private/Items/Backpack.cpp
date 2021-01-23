#include "MyProject.h"
#include "Backpack.h"
#include "ItemManager.h"
#include "LevelSaveStructs.h"
#include "BaseHero.h"
#include "ItemDelegateContext.h"
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

bool UBackpack::AddItem(FMyItem& newItem)
{
   check(newItem.id > 0);

   const bool bIsNewItemStackable = UItemManager::Get().GetItemInfo(newItem.id)->isStackable;
   if(bIsNewItemStackable)
   {
      return AddStackableItem(newItem);
   }
   return AddUnstackableItem(newItem);
}

bool UBackpack::AddItem(FMyItem&& newItem)
{
   return AddItem(newItem);
}

bool UBackpack::AddStackableItem(FMyItem& newItem)
{
   // Loop through the items we have
   for(FMyItem& i : items)
   {
      if(i.id == newItem.id && i.count != STACK_MAX)
      {
         // If this item slot's stack is not filled up by our new additions
         if(i.count + newItem.count <= STACK_MAX)
         {
            i.count += newItem.count;
            newItem.count = 0;
            CallBackpackUpdateDelegate();
            return true;
         }
         // Else fill up the slot and find another one to try and fill
         newItem.count -= STACK_MAX - i.count;
         i.count = STACK_MAX;
         CallBackpackUpdateDelegate();
      }
   }

   // We filled up existing slots, so now we check for empty slots
   while(newItem.count > 0)
   {
      if(items.Num() >= backpack_max_items) // If there are extra slots in the backpack
      {
         return false;
      }

      if(const int emptySlotIndex = FindEmptySlot(); emptySlotIndex != -1)
      {
         const int howManyItemsAdded = FMath::Min(STACK_MAX, newItem.count);
         items.Insert(emptySlotIndex, FMyItem(newItem.id, howManyItemsAdded));
         newItem.count -= howManyItemsAdded;
      }
   }

   return true;
}

bool UBackpack::AddUnstackableItem(FMyItem& newItem)
{
   if(items.Num() < backpack_max_items)
   {
      while(newItem.count > 0)
      {
         if(const int emptySlotIndex = FindEmptySlot(); emptySlotIndex != -1)
         {
            items.Insert(emptySlotIndex, FMyItem(newItem.id, 1));
            newItem.count -= 1;
         } else
         {
            return false;
         }
      }
      CallBackpackUpdateDelegate();
      return true;
   }

   return false;
}

bool UBackpack::IsItemStackable(int itemID)
{
   return UItemManager::Get().GetItemInfo(itemID)->isStackable;
}

void UBackpack::CallBackpackUpdateDelegate()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnBackpackUpdated().Broadcast(GetOuter());
}

bool UBackpack::AddItemToSlot(FMyItem& newItem, int slot)
{
   check(newItem.id > 0);
   check(newItem.count <= STACK_MAX);
   check(IsItemStackable(newItem.id) || newItem.count == 1); // Ensure this item is stackable or it is just a single item
   check(slot >= 0 && slot <= backpack_max_items);

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
               if(items[slot].count + newItem.count < STACK_MAX) // if we have enough room in our stack to add these items
               {
                  items[slot].count += newItem.count;
                  CallBackpackUpdateDelegate();
                  return true;
               } // if we don't have enough room we add the item to a full slot, and the rest of the item is still in our cursor
               newItem.count -= STACK_MAX - items[slot].count;
               items[slot].count = STACK_MAX;
               CallBackpackUpdateDelegate();
               return false;
            }
         }
      }
      return false;
   }
   
   // New slot just insert whatever logical item we have
   items.Insert(slot, FMyItem(newItem.id, newItem.count));
   CallBackpackUpdateDelegate();
   return true;
}

bool UBackpack::AddItems(TArray<FMyItem>& newItems)
{
   // Don't need to check for size of our backpack since newItems may contain stackable items
   for(FMyItem& newIt : newItems)
   {
      if(!AddItem(newIt))
         return false;
   }
   return true;
}

bool UBackpack::RemoveItem(const FMyItem& itemToRemove)
{
   const int newID       = itemToRemove.id;
   const int removeCount = itemToRemove.count;

   check(newID > 0);

   if(items.Num() > 0)
   {
      const int index = FindItem(newID);
      if(index == -1)
         return false;

      if(IsItemStackable((itemToRemove.id)))
      {
         if(items[index].count > removeCount)
         {
            items[index].count -= removeCount;
         } else
         {
            EmptySlot(index);
         }
      }
   }
   CallBackpackUpdateDelegate();
   return true;
}

bool UBackpack::RemoveItemAtSlot(const int slot, const int removeCount)
{
   if(items.Num() > 0)
   {
      if(items.IsAllocated(slot))
      {
         if(UItemManager::Get().GetItemInfo(items[slot].id)->isStackable && items[slot].count > removeCount)
         {
            items[slot].count -= removeCount;
         } else
         {
            items.RemoveAt(slot);
         }
         CallBackpackUpdateDelegate();
         return true;
      }
   }
   return false;
}

bool UBackpack::RemoveItems(const TArray<FMyItem>& itemsToRemove)
{
   bool success = true;
   for(const FMyItem& newIt : itemsToRemove)
   {
      if(!RemoveItem(newIt))
         success = false;
   }
   return success;
}

void UBackpack::EmptySlot(const int slot)
{
   items.RemoveAtUninitialized(slot);
   CallBackpackUpdateDelegate();
}

void UBackpack::EmptyAll()
{
   items.Empty(backpack_max_items);
   items.Reserve(backpack_max_items);
   CallBackpackUpdateDelegate();
}

bool UBackpack::TransferItems(UBackpack* otherPack, int transferSlot)
{
   if(otherPack->items.IsAllocated(transferSlot) && otherPack->items.Num() < backpack_max_items)
   {
      FMyItem* otherItem = &otherPack->items[transferSlot];

      if(otherItem && otherItem->id > 0)
      {
         const int initialItemCount = otherItem->count;
         if(AddItem(*otherItem)) // Check if successfully transferred everything
         {
            otherPack->EmptySlot(transferSlot);
         } // We have leftovers
         otherPack->RemoveItemAtSlot(transferSlot, initialItemCount - otherItem->count);
         
         CallBackpackUpdateDelegate();
         GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnBackpackUpdated().Broadcast(otherPack->GetOuter());
         return true;
      }
   }
   return false;
}

void UBackpack::SwapItems(UBackpack* otherPack, const int slot1, const int slot2)
{
   if(otherPack->items.IsAllocated(slot2))
   {
      if(items.IsAllocated(slot1)) // if there's an item in the slot we want to swap
      {
         const FMyItem itemFromOtherPack = otherPack->items[slot2];
         otherPack->items[slot2]         = items[slot1];
         items[slot1]                    = itemFromOtherPack;
      } else // if not, just insert an item there
      {
         items.Insert(slot1, otherPack->items[slot2]);
         otherPack->EmptySlot(slot2);
      }
      CallBackpackUpdateDelegate();
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnBackpackUpdated().Broadcast(otherPack->GetOuter());
   }
}

FMyItem UBackpack::GetItem(const int slot) const
{
   if(items.IsAllocated(slot)) { return items[slot]; }
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
      if(!items.IsAllocated(i)) { return i; }
   }
   return -1;
}

bool UBackpack::IsEmptySlot(const int slotIndex) const
{
   check(static_cast<unsigned>(slotIndex) < static_cast<unsigned>(GetItemMax()))
   return !items.IsAllocated(slotIndex) ? true : false;
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
      if((*it).id == itemID)
         return it.GetIndex();
   }
   return -1;
}

int UBackpack::FindItemCount(int itemID) const
{
   int itemCount = 0;
   for(auto it = items.CreateConstIterator(); it; ++it)
   {
      if((*it).id == itemID)
         itemCount += (*it).count;
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

void UBackpack::BeginDestroy()
{
   Super::BeginDestroy();
}
