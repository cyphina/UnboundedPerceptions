#pragma once
#include "Item.h"
#include "BackpackStructs.h"
#include "Backpack.generated.h"

struct FBackpackSaveInfo;

/**
 * @brief Contains all the items a hero is holding. \n
 * TODO: Maybe in the future we'll use the dirty flag pattern and update the inventory the frame after things get dirtied.
 */
UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UBackpack : public UObject
{
   GENERATED_BODY()

   TSparseArray<FMyItem> items;

   static const int STACK_MAX          = 99;            // max number of items in a stack
   static const int INIT_CAPACITY      = 10;            // initial capacity
   int              backpack_max_items = INIT_CAPACITY; // max number of item slots (set to 40 in base hero)

public:
   static UBackpack* CreateBackpack(UObject* outer, const int itemMax);

   /**Adds item to specified slot.  Returns number of items left (if new item is stackable and a place isn't found for it).
    * Only works on logical item stacks (single item if unstackable, or less than STACKMAX if stackable)
    * @param newItem - Data about new item that will be added
    * @param slot - Index for the newItem
    */
   UFUNCTION(BlueprintCallable)
   FBackpackUpdateResult AddItemToSlot(FMyItem newItem, int slot);

   /**
    * @brief Adds item to first available slot.  Returns number of items left (if new item is stackable and a place isn't found for it)
    * @param newItem - Data about new item that will be added
    * @return Returns the index (or indices for stackable items) of the slot(s) the item was added to if successful, or returns an empty array.
    */
   UFUNCTION(BlueprintCallable)
   FBackpackUpdateResult AddItem(FMyItem newItem);

   /**Add several items to the backpack.  On sucess, the array passed in will be full of 0 count items. On fail there will be items with remaining counts > 0
    * @param newItems - Data about each new item to be added
    * @return - Returns the index (or indices for stackable items) of the slot(s) the item was added to if successful, or returns an empty array.
    */
   UFUNCTION(BlueprintCallable)
   TArray<FBackpackUpdateResult> AddItems(const TArray<FMyItem>& newItems);

   /**Remove an item at a certain slot
    * @param slot - Slot of the item to be removed
    * @param removeCount - How many should be removed
    */
   UFUNCTION(BlueprintCallable)
   FBackpackUpdateResult RemoveItemAtSlot(const int slot, const int removeCount = 1);

   /**Remove particular item from inventory
    * @param itemToRemove - Information pertaining to removal.  Stores count (how many to remove), and id (what kind of item to remove)
    */
   UFUNCTION(BlueprintCallable)
   FBackpackUpdateResult RemoveItem(FMyItem itemToRemove);

   /**Attempts to remove items in itemsToRemove from the backpack.
    * @param itemsToRemove - Array listing what items to remove and how much of that item should be removed
    */
   UFUNCTION(BlueprintCallable)
   TArray<FBackpackUpdateResult> RemoveItems(const TArray<FMyItem>& itemsToRemove);

   /**Empties a slot*/
   UFUNCTION(BlueprintCallable)
   FBackpackUpdateResult EmptySlot(const int slot);

   /**Clears the backpack*/
   UFUNCTION(BlueprintCallable)
   void EmptyAll();

   /**
    * Transfers an item from a specific slot in another backpack to the first available slot in this one (like when we click on items in storage)
    * @param otherPack - Reference to backpack we're moving items from
    * @param transferSlot - Slot index of the item we're moving
    * @return - Returns a pair containing results of adding the item from the other pack to this pack, and removing the item from the other pack
    */
   TPair<FBackpackUpdateResult, FBackpackUpdateResult> TransferItems(UBackpack* otherPack, const int transferSlot);

   /**
    * Swap two items location in separate backpacks.  Swaps item from the other pack to this one.
    * Used when dragging and dropping items from one backpack slot to another, or from a backpack slot to a storage slot.
    * @param otherPack - The other backpack which swaps an item to us
    * @param slot1 - Index of the slot in this backpack which is going to be swapped
    * @param slot2 - Index of the slot in the other backpack which we're swapping to
    */
   void SwapItems(UBackpack* otherPack, const int slot1, const int slot2);

   /** Gets information about the item in a slot. Returns a copy so it can't actually modify the backpack
    * @param slot - The slot of the item to grab
    * @return - Returns default FMyItem() if there was nothing in that slot
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
   FMyItem GetItem(const int slot) const;

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
   int FindItem(const int itemID) const;

   /**Returns how many of an item is in the backpack
    * @param itemID - ID of the item to count
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
   int FindItemCount(const int itemID) const;

   /**Is this slot free in our backpack?
    * @param slotIndex - Slot to check
    */
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Read Inventory Info")
   bool IsEmptySlot(const int slotIndex) const;

   /** Refers to constant denoting how large a stack of items can go to so we can use it in BP*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
   int MAX_STACK() const { return STACK_MAX; }

   /** Max capacity of backpack*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
   int GetItemMax() const { return backpack_max_items; }

   /** Change max capacity of backpack. Can only increase max capacity (no decreasing at runtime) */
   UFUNCTION(BlueprintCallable, Category = "Inventory Information")
   void SetItemMax(const int newMax);

   /** Returns how many items in inventory*/
   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Information")
   int Count() const;

   FMyItem& operator[](int backpackIndex) { return items[backpackIndex]; }

   /**Copies backpack information into a struct for saving*/
   void SaveBackpack(FBackpackSaveInfo& backpackInfo);

   /**Loads backpack information from a saving struct*/
   void LoadBackpack(FBackpackSaveInfo& backpackInfo);

protected:
   void BeginDestroy() override;

private:
   UBackpack();

   /** Find the first empty slot in the backpack */
   int FindEmptySlot() const;

   FBackpackUpdateResult AddStackableItem(FMyItem itemToAdd);
   FBackpackUpdateResult AddUnstackableItem(FMyItem itemToAdd);

   bool                  IsItemStackable(int itemID);
};
