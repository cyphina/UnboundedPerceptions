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
   UStoreInventory();

   UFUNCTION()
   AShopNPC* GetShopkeeper() const { return shopkeeper; }

   void UseItemAtInventorySlot_Implementation(int32 iSlot) override;

protected:
   void NativeOnInitialized() override;
   
private:
   /** The player bought a single item */
   UFUNCTION()
   bool OnItemPurchased() const;

   /** The player bought a multiple of the same item (confirmation box opens and player inputs number they would like to buy) */
   UFUNCTION()
   bool OnItemsPurchased(FString howManyItems);

   bool EnoughFunds(int numPurchasing) const;
   bool OnWidgetAddToViewport_Implementation() override;

   AShopNPC*   shopkeeper;
   ABaseHero** interactingHero;

   UFUNCTION()
   UBackpack* const GetInteractingHeroBackpack() const;
   
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
