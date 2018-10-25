// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory.h"
#include "StoreInventory.generated.h"

/**
 * Inventory for shops
 */

class AShopNPC;
class UBackpack;
struct FMyItem;
struct FItemPrice;

UCLASS()
class MYPROJECT_API UStoreInventory : public UInventory
{
   GENERATED_BODY()

   static FText NotEnoughItemsText;
   static FText NotEnoughMoneyText;
   static FText ensurePurchaseText;
   static FText confirmTitleText;

   int         itemSlot            = -1;
   FItemPrice* itemPrice           = nullptr;
   UBackpack*  interactingHeroPack = nullptr;
   int         itemToBuy           = -1;

   UFUNCTION()
   bool OnItemPurchased();

   UFUNCTION()
   bool OnItemsPurchased(FString howManyItems);

   bool EnoughFunds(int numPurchasing);

 public:
   UPROPERTY(BlueprintReadOnly)
   AShopNPC* shopkeeper;

   void UseItemAtInventorySlot_Implementation(int32 iSlot) override;
};
