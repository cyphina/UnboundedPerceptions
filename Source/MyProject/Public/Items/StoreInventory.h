#pragma once

#include "CoreMinimal.h"

#include "BackpackStructs.h"
#include "Items/Inventory.h"
#include "StoreInventory.generated.h"

class AShopNPC;
class UBackpack;
struct FMyItem;
struct FItemPrice;

/**
* Inventory for shops to buy things in game for your units!
*/
UCLASS()
class MYPROJECT_API UStoreInventory : public UInventory
{
   GENERATED_BODY()

public:
<<<<<<< HEAD
   UStoreInventory();

   UFUNCTION()
   AShopNPC* GetShopkeeper() const { return shopkeeper; }
=======
   UFUNCTION()
   AShopNPC* GetShopkeeper() const { return shopkeeper; }
   
protected:
   void NativeOnInitialized() override;
   bool OnWidgetAddToViewport_Implementation() override;

private:
   void OnItemPurchased(const ABaseHero* heroRef, const FBackpackUpdateResult& addItemResult, const TArray<FBackpackUpdateResult>& removeItemsResults);
>>>>>>> componentrefactor

   void UseItemAtInventorySlot_Implementation(int32 iSlot) override;

<<<<<<< HEAD
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
=======
   AShopNPC* shopkeeper;
>>>>>>> componentrefactor
};
