// Created 1/25/21 08:59 AM

#pragma once
#include "Item.h"

/**
 * Represents the result of one backpack update operation.
 * Can be used to implement memento one day... if I stored how much each slot was updated b
 */
USTRUCT(NoExport, BlueprintType)
struct FBackpackUpdateResult
{
   friend class UBackpack;

   FBackpackUpdateResult() : updatedBackpackIndices(TArray<int>()), bSuccessfulOperation(false), itemId(-1), numUpdatedItemsRequested(0), numUpdatedItemsRemaining(0) {}

   explicit FBackpackUpdateResult(const int newItemCount) : FBackpackUpdateResult() { this->numUpdatedItemsRemaining = newItemCount; }

   explicit FBackpackUpdateResult(const FMyItem& itemBeforeUpdate) : FBackpackUpdateResult()
   {
      itemId                   = itemBeforeUpdate.id;
      numUpdatedItemsRequested = itemBeforeUpdate.count;
   }

   FBackpackUpdateResult(const TArray<int>& updated_backpack_indices, bool b_successful_operation, int item_id, int num_updated_items_requested,
                         int num_updated_items_remaining) :
       updatedBackpackIndices(updated_backpack_indices),
       bSuccessfulOperation(b_successful_operation), itemId(item_id), numUpdatedItemsRequested(num_updated_items_requested),
       numUpdatedItemsRemaining(num_updated_items_remaining)
   {
   }

   /** Number of items that were handled sucessfully in this update */
   int GetNumberItemsHandled() const { return numUpdatedItemsRequested - numUpdatedItemsRemaining; }

   operator bool() const { return bSuccessfulOperation; }

   explicit operator int() = delete;

   UPROPERTY(BlueprintReadOnly)
   TArray<int> updatedBackpackIndices;

   UPROPERTY(BlueprintReadOnly)
   bool bSuccessfulOperation;

   /** Id of the item that is involved with this backpack update*/
   UPROPERTY(BlueprintReadOnly)
   int itemId;

   /** How many items did this request ask to update */
   UPROPERTY(BlueprintReadOnly)
   int numUpdatedItemsRequested;

   /** How many items (out of the amount we passed in) are left not updated? (anything > 0 means this operation failed) */
   UPROPERTY(BlueprintReadOnly)
   int numUpdatedItemsRemaining;

 private:
   FBackpackUpdateResult& operator()(const TArray<int>& updatedIndices, bool bSuccess, int numUpdatedItemsRemainingCount)
   {
      updatedBackpackIndices   = updatedIndices;
      bSuccessfulOperation     = bSuccess;
      numUpdatedItemsRemaining = numUpdatedItemsRemainingCount;
      return *this;
   }
};
