#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/IntimateNPC.h"
#include "Backpack.h"
#include "ShopNPC.generated.h"

/**
 * Depending on your friendship, shopkeepers can sell more things like in TWEWY.  This will probably be handled inside the triggers in IntimateNPC
 */

USTRUCT(Blueprintable, NoExport)
struct FItemPrice
{
   FItemPrice() :
      money(0), tradeItems(TArray<int>())
   {
   }

   /** How much money this item costs */
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   int money;

   /** How many items to trade in for this item */
   UPROPERTY(EditAnywhere, BlueprintReadWrite)
   TArray<FMyItem> tradeItems;
};

UCLASS()
class MYPROJECT_API AShopNPC : public AIntimateNPC
{
   GENERATED_BODY()

public:
   UFUNCTION(BlueprintCallable, BlueprintPure)
   const FItemPrice& BP_GetItemPrice(int itemID) const;

   /**
   * Accessor to gets an item's price
   */
   const FItemPrice* GetItemPrice(int itemID) const;

   void OnAskToPurchaseItem(int purchaseItemSlotIndex);

   UBackpack* GetItemsToSell() const { return itemsToSellBackpack; }

protected:
   void BeginPlay() override;

   UPROPERTY(EditAnywhere, Category = "Shop Items")
   TArray<FMyItem> itemsToSell;

private:
   /** When the player buys a single item and clicks on the confirmation box */
   bool OnItemPurchased(const FMyItem itemPurchased, const FItemPrice& itemPrice) const;

   /** When the player buys a stackable item and inputs some value */
   bool OnItemsPurchased(const FMyItem itemToBuy, const FItemPrice& itemPrice, FString howManyItems);

   void       SetupAppropriateView() override;
   bool       EnoughFunds(const FItemPrice& itemPrice, int numPurchasing) const;
   UBackpack* GetInteractingHeroBackpack() const;

   TArray<FBackpackUpdateResult> PayItemReqsForEachItemPurchased(const FItemPrice& itemPrice, int numberPurchased) const;
   
   /**
    * Lists sellable items, and maps them to price
    */
   UPROPERTY(EditAnywhere, Category = "Shop Items")
   TMap<int, FItemPrice> itemPrices;

   /**
   * Backpack containing the items that the shopkeeper will sell and how many the shopkeeper can sell before running out of stock
   * TODO: Setup some kind of list of what the shopkeeper sells based on what part of the story has occured
   */
   UPROPERTY()
   UBackpack* itemsToSellBackpack;

   static const FText NotEnoughItemsText;
   static const FText NotEnoughMoneyText;
   static const FText ensurePurchaseText;
   static const FText ensurePurchaseSingleText;
   static const FText confirmTitleText;

   int purchaseItemId = -1;
};
