// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Item.h"
#include "Backpack.generated.h"
/**
 * Container class for items
 * Every hero has a backpack. Implemented using a sparse array
 */

struct FBackpackSaveInfo;

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UBackpack : public UObject
{
   GENERATED_BODY()

   TSparseArray<FMyItem> items;

   const int STACKMAX = 99; // max number of items in a stack
   int       itemMax  = 100;

 public:

   UBackpack();

   /**Adds item to specified slot.  Returns number of items left (if new item is stackable and a place isn't found for it)
    * @param newItem - Data about new item that will be added
    * @param slot - Index for the newItem
    */
   UFUNCTION(BlueprintCallable)
   int AddItemToSlot(FMyItem newItem, int slot);

   /**Adds item to first available slot.  Returns number of items left (if new item is stackable and a place isn't found for it)
    * @param newItem - Data about new item that will be added
    */
   UFUNCTION(BlueprintCallable)
   int AddItem(FMyItem newItem);

   /**Add several items to the backpack.  On sucess, returns an empty array.  On failure, returns array passed in with leftover items
    * @param newItems - Data about each new item to be added
    */
   UFUNCTION(BlueprintCallable)
   TArray<FMyItem> AddItems(TArray<FMyItem> newItems);

   /**Remove an item at a certain slot
    * @param slot - Slot of the item to be removed
    * @param removeCount - How many should be removed
    */
   UFUNCTION(BlueprintCallable)
   bool RemoveItemAtSlot(int slot, int removeCount = 0);

   /**Remove particular item from inventory
    * @param itemToRemove - Information pertaining to removal.  Stores count (how many to remove), and id (what kind of item to remove)
    */
   UFUNCTION(BlueprintCallable)
   bool RemoveItem(const FMyItem& itemToRemove);

   /**Attempts to remove items in itemsToRemove from the backpack.
    * @param itemsToRemove - Array listing what items to remove and how much of that item should be removed
    */
   UFUNCTION(BlueprintCallable)
   bool RemoveItems(const TArray<FMyItem> itemsToRemove);

   /**Empties a slot*/
   UFUNCTION(BlueprintCallable)
   void EmptySlot(int slot);

   /**Clears the backpack*/
   UFUNCTION(BlueprintCallable)
   void EmptyAll();

   /**Transfers an item from one backpack to another.  Returns how many items leftover (if this pack is full)
    * @param otherPack - Reference to backpack we're moving items from
    * @param transferSlot - Slot index of the item we're moving
    */
   UFUNCTION(BlueprintCallable)
   void TransferItems(UBackpack* otherPack, int transferSlot);

   /**Swap two items location in seperate backpacks.  Swaps item from the other pack to this one
    * @param otherPack - The other backpack which swaps an item to us
    * @param slot1 - Index of the slot in this backpack which is going to be swapped
    * @param slot2 - Index of the slot in the other backpack which we're swapping to
    */
   UFUNCTION(BlueprintCallable)
   void SwapItems(UBackpack* otherPack, int slot1, int slot2);

   /**Item accessor
    * @param slot - The slot of the item to grab
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
   FMyItem GetItem(int slot) const;

   /**Problems returning pointers to non-uobjects in blueprints.  Only returns array of items that are "allocated" in bit array*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
   TArray<FMyItem> GetItems();

   /**Returns the slot indices of all the items currently in the backpack*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
   TArray<int> GetItemIndices();

   /**Returns index of an item if an item is in the backpack, or -1 if it's not
    * @param itemID - ID of item to look for
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
   int FindItem(int itemID);

   /**Returns how many of an item is in the backpack
    * @param itemID - ID of the item to count
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
   int FindItemCount(int itemID);

   /**Find the first empty slot in the backpack*/
   int FindEmptySlot() const;

   /**Is this slot free in our backpack?
    * @param slotIndex - Slot to check
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
   FORCEINLINE bool IsEmptySlot(int slotIndex) const;

   /**Refers to constant denoting how large a stack of items can go to so we can use it in BP*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
   int MAX_STACK() const { return STACKMAX; }

   /**Max capacity of backpack*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
   FORCEINLINE int GetItemMax() const { return itemMax; }

   /**Change max capacity of backpack*/
   UFUNCTION(BlueprintCallable, Category = "Inventory Information")
   void SetItemMax(int newMax);

   /**Returns how many items in inventory*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
   FORCEINLINE int Count() const;

   FMyItem& operator[](int backpackIndex) { return items[backpackIndex]; }

   void SaveBackpack(FBackpackSaveInfo& backpackInfo);
};
