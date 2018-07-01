// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Backpack.h"
#include "ItemManager.h"

UBackpack::UBackpack()
{	
	items = TSparseArray<FMyItem>();
	items.Reserve(itemMax);
}

UBackpack::~UBackpack()
{
}

int UBackpack::AddItem(FMyItem newItem)
{
	FMyItem& newItemReference = newItem;
	int newID = newItemReference.id;
	int	newCount = newItemReference.count;

#if UE_EDITOR 
	check(newID > 0)
#endif

	if (UItemManager::Get().GetItemInfo(newID)->isStackable) //if this is a stackable item
	{
		for (FMyItem& i : items) //loop through items and see if there's one already inside
		{
			if (i.id == newID && i.count != STACKMAX) //if there's a matching slot with this type of item and it's not already full
			{
				if (i.count + newCount <= STACKMAX) //if the slot has enough room to add these additional items
				{
					i.count += newCount; //add the count of the newitems to our original item count
					return 0;
				}
				else //if this slot doesn't have enough room for all the additional item amount we're trying to add
				{
					newCount = newCount - (99 - i.count); //lower the item count since we added some items
					i.count = STACKMAX; //fill up the slot 
					return AddItem(newItem); //try to add the rest of the item again to a new slot
				}
			}
		}
	}

	//if item is not stackable, or there is no other instances of it or the instances of it are already full, just add items to a new slot
	if (items.Num() < itemMax) //if there's extra room in the 
	{
		//Insert into the empty slot the item
		items.Insert(FindEmptySlot(), newItem);
		return 0;
	}

	return newCount;
}

int UBackpack::AddItemToSlot(FMyItem newItem, int slot) //<--FIX HERE
{
	int newID = newItem.id;
	int	newCount = newItem.count;

#if UE_EDITOR 
	check(newID > 0)
#endif

	check(slot >= 0 && slot <= itemMax); //is this a valid slot
	bool isSlotAllocated = items.IsAllocated(slot);

	if (isSlotAllocated && items[slot].id == newID && UItemManager::Get().GetItemInfo(items[slot].id)->isStackable) //if we have an item in our slot already
	{
		if (items[slot].count + newCount < STACKMAX) //if we have enough room in our stack to add these items
		{
			items[slot].count += newCount;
			return 0;
		}
		else //if we don't have enough room we add the item to a full slot, and the rest of the item is still in our cursor
		{
			newCount = items[slot].count + newCount - 99;
			items[slot].count = 99;
			return newCount;
		}
	}
	else //if this item is stackable
	{
		if (isSlotAllocated) //Already taken
			return newCount;
		else
			items.Insert(slot, newItem);

		return 0;
	}
}
	
TArray<FMyItem> UBackpack::AddItems(TArray<FMyItem> newItems)
{
	if (items.Num() <= itemMax - newItems.Num())
	{
		for (FMyItem& newIt : newItems)
		{
			if(AddItem(newIt) != 0)
				return newItems;
		}
	}
	return TArray<FMyItem>();
}

bool UBackpack::RemoveItem(const FMyItem& itemToRemove)
{
	int newID = itemToRemove.id;
	int newCount = itemToRemove.count;

#if UE_EDITOR 
	check(newID > 0)
#endif

	if(items.Num() > 0)
	{
		int index = FindItem(newID);

		if (index == -1) return false;

		if (items[index].count > newCount)
		{
			items[index].count -= newCount;
			return true;
		}
		else
			EmptySlot(index);
	}
	return false;
}

bool UBackpack::RemoveItemAtSlot(int slot, int removeCount)
{
	if (items.Num() > 0)
	{
		if (items.IsAllocated(slot))
		{
			if(UItemManager::Get().GetItemInfo(items[slot].id)->isStackable && items[slot].count > removeCount)
			{
				items[slot].count -= removeCount;
			}
			else
			{
				items.RemoveAtUninitialized(slot);
			}
			return true;
		}
	}
	return false;
}

bool UBackpack::RemoveItems(const TArray<FMyItem> itemsToRemove)
{
	bool success = true;
	for (const FMyItem& newIt : itemsToRemove)
	{
		if(!RemoveItem(newIt))
			success = false;
	}
	return success;
}

FORCEINLINE void UBackpack::EmptySlot(int slot)
{
	items.RemoveAtUninitialized(slot);
}

FORCEINLINE void UBackpack::EmptyAll()
{
	items.Empty(itemMax);
}

void UBackpack::TransferItems(UBackpack* otherPack, int transferSlot)
{
	if (otherPack->items.IsAllocated(transferSlot))
	{
		int initalItemCount = otherPack->GetItem(transferSlot).count;
		int numItemsLeft = AddItem(otherPack->GetItem(transferSlot));

		if(numItemsLeft == 0) //sucessfully transferred everything
		{
			otherPack->EmptySlot(transferSlot);
		}
		else //we have leftovers
		{
			otherPack->RemoveItemAtSlot(initalItemCount - numItemsLeft);
		}
	}
}

void UBackpack::SwapItems(UBackpack* otherPack, int slot1, int slot2)
{
	if (otherPack->items.IsAllocated(slot1))
	{
		if (items.IsAllocated(slot2)) //if there's an item in the slot we want to swap
		{
			FMyItem item1 = otherPack->items[slot1];
			items[slot1] = items[slot2];
			items[slot2] = item1;
		}
		else //if not, just insert an item there
		{
			items.Insert(slot2, otherPack->items[slot1]);
			otherPack->items.RemoveAt(slot1);	
		}
	}
}

FMyItem UBackpack::GetItem(int slot) const
{
	if (slot >= 0 && slot < itemMax)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::FromInt(slot));
		//assert that we don't get some kind of error for going out of range
		if (items.IsAllocated(slot))
		{
#if UE_EDITOR 
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, items[slot].description.ToString());
#endif
			return items[slot];
		}
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
	for (auto it = items.CreateIterator(); it; ++it)
	{
		itemsToGet.Add(*it);
	}
	return itemsToGet;
}

TArray<int> UBackpack::GetItemIndices()
{
	TArray<int> indicesToGet;
	for (auto it = items.CreateIterator(); it; ++it)
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

bool UBackpack::IsEmptySlot(int slotIndex) const
{
	return !items.IsAllocated(slotIndex) ? true : false;
}

FORCEINLINE void UBackpack::SetItemMax(int newMax)
{
	itemMax = newMax;
	items.Reserve(itemMax);
}

int UBackpack::FindItem(int itemID)
{
	for(auto it = items.CreateIterator(); it; ++it)
	{
		if ((*it).id == itemID)
			return it.GetIndex();
	}
	return -1;
}

int UBackpack::FindItemCount(int itemID)
{
	int itemCount = 0;
	for(auto it = items.CreateIterator(); it; ++it)
	{
		if ((*it).id == itemID)
			itemCount += (*it).count; 
	}
	return itemCount;
}

