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

 public:
   UFUNCTION()
   AShopNPC* GetShopkeeper() const { return shopkeeper; }

   void UseItemAtInventorySlot_Implementation(int32 iSlot) override;

 private:
   UFUNCTION()
   bool OnItemPurchased() const;

   UFUNCTION()
   bool OnItemsPurchased(FString howManyItems);

   bool EnoughFunds(int numPurchasing) const;
   bool OnWidgetAddToViewport_Implementation() override;

   UPROPERTY()
   AShopNPC* shopkeeper;

   UPROPERTY()
   UBackpack* interactingHeroPack = nullptr;

   UPROPERTY()
   class AHUDManager* hudManagerRef;

   static const FText NotEnoughItemsText;
   static const FText NotEnoughMoneyText;
   static const FText ensurePurchaseText;
   static const FText ensurePurchaseSingleText;
   static const FText confirmTitleText;

   int         itemSlot  = -1;
   FItemPrice* itemPrice = nullptr;
   int         itemToBuy = -1;
};
