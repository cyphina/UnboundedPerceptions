// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Item.h"
#include "Components/ActorComponent.h"
#include "Backpack.generated.h"
/**
 * Container class for items
 * Every hero has a backpack. Implemented using a sparse array 
 */
UCLASS(BlueprintType, ClassGroup=InventoryContainer, meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UBackpack : public UActorComponent
{
	GENERATED_BODY()
	TSparseArray<UMyItem*> items;

	const int STACKMAX = 99; //max number of items in a stack
	int itemMax = 100;

public:

	UBackpack();
	~UBackpack();

	/**returns true if all the items were successfully added*/
	//UFUNCTION(BlueprintCallable)
	bool				AddItemToSlot(UMyItem* newItem, int slot);

	/**Adds item to last slot in inventory*/
	UFUNCTION(BlueprintCallable)
	bool				AddItem(UMyItem* newItem);

	/**Add several items to the backpack: WARNING SHOULDNT WORK SINCE WE CAN'T SIMPLY ADD (Have to revisit this)*/
	bool				AddItems(TArray<UMyItem*> newItems); 
	
	/**Remove an item at a certain slot*/
	UFUNCTION(BlueprintCallable)
	bool				RemoveItemAtSlot(int slot);

	/**Remove particular item from inventory*/
	bool				RemoveItem(UMyItem* itemToRemove);

	/**Remove items from the backpack*/
	bool				RemoveItems(TArray<UMyItem*> itemsToRemove); 

	/**Clears the backpack*/
	bool				RemoveAll(); 

	/**Swap two items location in backpack*/
	void				SwapItems(UBackpack* otherPack, int slot1, int slot2);

	/**Item accessor*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
	UMyItem*			GetItem(int slot); 

	/**Problems returning pointers to non-uobjects in blueprints.  Only returns array of items that are "allocated" in bit array*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info") 
	TArray<UMyItem*>	GetItems();

	/**Returns the slot indices of all the items currently in the backpack*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info") 
	TArray<int>			GetItemIndices();

	/**Find if an item is in the backpack
	 * @param name - Name of item to look for
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info") 
	int					FindItemByName(FString name);

	/**Find the first empty slot in the backpack*/
	int					FindEmptySlot() const;

	/**Refers to constant denoting how large a stack of items can go to so we can use it in BP*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
	int					MAX_STACK() const { return STACKMAX; }

	/**Max capacity of backpack*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information") 
	int					GetItemMax() const { return itemMax; }

	/**Change max capacity of backpack*/
	UFUNCTION(BlueprintCallable, Category = "Inventory Information") 
	void				SetItemMax(int newMax);
	
	/**how many items in inventory*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
	int					Count() const; 

private:
	int					FindItem(UMyItem* item);
};
