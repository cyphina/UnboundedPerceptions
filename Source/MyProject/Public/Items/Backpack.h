// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Item.h"
#include "Components/ActorComponent.h"
#include "Backpack.generated.h"
/**
 * Container class for items
 * Every hero has a backpack. 
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

	//returns true if all the items were successfully added
	//UFUNCTION(BlueprintCallable)
	bool AddItemToSlot(UMyItem* newItem, int slot);

	//add item to last slot in inventory
	UFUNCTION(BlueprintCallable)
	bool AddItem(UMyItem* newItem);

	//add several items to the backpack WARNING SHOULDNT WORK SINCE WE CAN'T SIMPLY ADD
	bool AddItems(TArray<UMyItem*> newItems); 

	UFUNCTION(BlueprintCallable)
	bool RemoveItemAtSlot(int slot);

	//remove particular item from inventory
	bool RemoveItem(UMyItem* itemToRemove);

	//remove items from the backpack
	bool RemoveItems(TArray<UMyItem*> itemsToRemove); 

	//clear the backpack
	bool RemoveAll(); 

	//Swap two items location in backpack.  Exposed through inventory
	void SwapItems(UBackpack* otherPack, int slot1, int slot2);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
		UMyItem* GetItem(int slot); //item accessor

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info") //Problems returning pointers to non-uobjects in blueprints.  Only returns array of items that are "allocated" in bit array
		TArray<UMyItem*> GetItems();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info") //this is the slots of all the items
		TArray<int> GetItemIndices();

	int FindEmptySlot() const;

	//refers to our constant so we can use it in BP
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
		int MAX_STACK() const { return STACKMAX; }

	//max capacity of backpack
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information") 
		int GetItemMax() const { return itemMax; }

	//change max capacity of backpack
	UFUNCTION(BlueprintCallable, Category = "Inventory Information") 
		void SetItemMax(int newMax);
	
	//how many items in inventory?
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
		int Count() const; 
private:
	int FindItem(UMyItem* item);
};
