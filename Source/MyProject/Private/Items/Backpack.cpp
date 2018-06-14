// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Backpack.h"

UBackpack::UBackpack()
{	
	items = TSparseArray<UMyItem*>();
	//items.Reserve(itemMax);
}

UBackpack::~UBackpack()
{
}

bool UBackpack::AddItem(UMyItem* newItem)
{
#if UE_EDITOR 
	if (!newItem)
		return false;
#endif
	if (newItem->itemInfo.isStackable)
	{
		for (UMyItem* i : items) //loop through items and see if there's one already inside
		{
			if (i->itemInfo.name.EqualTo(newItem->itemInfo.name) && i->itemInfo.isStackable) //if we have this item already and it is stackable
			{
				if (i->itemInfo.count + newItem->itemInfo.count <= STACKMAX) //if the number of items we have is less than the max stack
				{
					i->itemInfo.count += newItem->itemInfo.count; //add the count of the newitems to our original item count
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::FromInt(i.count));
					return true;
				}
				else //make this stack full and try to find another
				{
					newItem->itemInfo.count = 99 - i->itemInfo.count; //the newItem now has less in it
					i->itemInfo.count = 99; //but our stack is 99
				}
			}
		}
	}
	if (items.Num() < itemMax) //if item is not stackable, or there is no other instances of it, just add items to a new slot
	{	
#if UE_EDITOR 
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, newItem->itemInfo.name.ToString());
#endif
		TArray<int> indices = GetItemIndices();
		if (indices.Num() > 0)
		{
			int i = 0;
			for (; i < itemMax; ++i)
			{
				if (!items.IsAllocated(i))
					break;
			}
			items.Insert(i, newItem); //else we don't have this item already so lets add it in
		}
		else
			items.Add(newItem);

		return true;
	}
	return false;
}

bool UBackpack::AddItemToSlot(UMyItem* newItem, int slot) //<--FIX HERE
{
#if UE_EDITOR 
	if (!newItem)
		return false;
#endif
	check(slot >= 0 && slot <= itemMax); //is this a valid slot
	if (items.IsAllocated(slot) && items[slot]->itemInfo.name.EqualTo(newItem->itemInfo.name) && items[slot]->itemInfo.isStackable) //if we have an item in our slot already
	{
		if (items[slot]->itemInfo.count + newItem->itemInfo.count < STACKMAX) //if we have enough room in our stack to add these items
		{
			items[slot]->itemInfo.count += newItem->itemInfo.count;
			return true;
		}
		else //if we don't have enough room we add the item to a full slot, and the rest of the item is still in our cursor
		{
			newItem->itemInfo.count = items[slot]->itemInfo.count + newItem->itemInfo.count - 99;
			items[slot]->itemInfo.count = 99;
			return false;
		}
	}
	else //if there's nothing in that slot
	{
		if (items.IsAllocated(slot))
			items[slot] = newItem;
		else
			items.Insert(slot, newItem);
		return true;
	}
	return false;
}
	
bool UBackpack::AddItems(TArray<UMyItem*> newItems)
{
	if (items.Num() <= itemMax - newItems.Num())
	{
		for (UMyItem* newIt : newItems)
		{
			AddItem(newIt);
		}
		return true;
	}
	return false;
}

bool UBackpack::RemoveItem(UMyItem* itemToRemove)
{
#if UE_EDITOR 
	if (!itemToRemove)
		return false;
#endif
	if(items.Num() > 0)
	{
		int index = FindItem(itemToRemove);

		if (index == -1) return false;

		if (items[index]->itemInfo.count > 1)
		{
			items[index]->itemInfo.count -= 1;
			return true;
		}
		else
			items.RemoveAtUninitialized(index);
	}
	return false;
}

bool UBackpack::RemoveItemAtSlot(int slot)
{
	if (items.Num() > 0)
	{
		if (items.IsAllocated(slot))
		{
			if(items[slot]->itemInfo.isStackable && items[slot]->itemInfo.count > 1)
			{
				items[slot]->itemInfo.count -= 1;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, FString::FromInt(slot));
				items.RemoveAtUninitialized(slot);
			}
			return true;
		}
	}
	return false;
}


bool UBackpack::RemoveItems(TArray<UMyItem*> itemsToRemove)
{
	if (items.Num() <= itemMax - itemsToRemove.Num())
	{
		for (UMyItem* newIt : itemsToRemove)
		{
			RemoveItem(newIt);
		}
		return true;
	}
	return false;
}

bool UBackpack::RemoveAll()
{
	return false;
}

void UBackpack::SwapItems(UBackpack* otherPack, int slot1, int slot2)
{
	if (otherPack->items.IsAllocated(slot1))
	{
		if (items.IsAllocated(slot2)) //if there's an item in the slot we want to swap
		{
			UMyItem* item1 = otherPack->items[slot1];
			items[slot1] = items[slot2];
			items[slot2] = item1;
		}
		else //if not, just insert an item there
		{
			items.Insert(slot2, otherPack->items[slot1]);
			otherPack->items.RemoveAt(slot1);	
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::FromInt(items[slot1].count));
		}
	}
}

UMyItem* UBackpack::GetItem(int slot)
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
	return nullptr;
}

int UBackpack::Count() const
{
	return items.Num();
}

TArray<UMyItem*> UBackpack::GetItems()
{
	TArray<UMyItem*> itemsToGet;
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

int UBackpack::FindItemByName(FString name)
{
	for (auto it = items.CreateIterator(); it; ++it)
	{
		if((*it)->itemInfo.name.ToString() == name)
		{
			return it.GetIndex();
		}
	}
	return -1;
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

void UBackpack::SetItemMax(int newMax)
{
	itemMax = newMax;
	items.Reserve(itemMax);
}

int UBackpack::FindItem(UMyItem* item)
{
	for(auto it = items.CreateIterator(); it; ++it)
	{
		if ((*it) == item)
			return it.GetIndex();
	}
	return -1;
}

