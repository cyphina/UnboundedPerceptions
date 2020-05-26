// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StoreInventory.h"
#include "UserInput.h"
#include "UI/HUDManager.h"

#include "RTSGameMode.h"
#include "QuestManager.h"

#include "UI/UserWidgets/RTSIngameWidget.h"
#include "Items/HeroInventory.h"

#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/ShopNPC.h"
#include "ItemManager.h"

FText UStoreInventory::NotEnoughItemsText = NSLOCTEXT("HelpMessages", "MisisngItems", "Missing required items for trade");
FText UStoreInventory::NotEnoughMoneyText = NSLOCTEXT("HelpMessages", "NotEnoughSqueezies", "You don't have enough squeezies...");
FText UStoreInventory::confirmTitleText   = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirmTitle", "Purchase");
FText UStoreInventory::ensurePurchaseText = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirm", "How many of these would you like to purchase?");

bool UStoreInventory::OnItemPurchased()
{
   interactingHeroPack->RemoveItems(itemPrice->items);
   CPC->GetBasePlayer()->money -= itemPrice->money;
   FMyItem newItemPurchased{itemToBuy, 1};
   CPC->GetBasePlayer()->heroInBlockingInteraction->backpack->AddItem(newItemPurchased);
   // Reload our inventory in case it is open
   CPC->GetHUDManager()->GetInventoryHUD()->LoadItems();
   // We obtained an item so tell the quest manager in case it is a requirement
   CPC->GetGameMode()->GetQuestManager()->OnItemPickup(newItemPurchased);
   return true;
}

bool UStoreInventory::OnItemsPurchased(FString howManyItems)
{
   if(howManyItems != "") {
      // Reads in number of items purchased from confirmation box
      int num = FCString::Atoi(*howManyItems);
      if(EnoughFunds(num)) {
         // If our backpack is not already full
         if(interactingHeroPack->FindEmptySlot() != INDEX_NONE) {
            // Remove the items necessary for the trade
            TArray<FMyItem> itemPriceItems = itemPrice->items;
            for(FMyItem& item : itemPriceItems) {
               item.count *= num;
            }
            interactingHeroPack->RemoveItems(itemPriceItems);

            // Remove the money
            CPC->GetBasePlayer()->money -= itemPrice->money * num;
            // Add the item to our inventory and reload our inventory. Also notify quests that we obtained a new item
            FMyItem newItemPurchased{itemToBuy, num};
            interactingHeroPack->AddItem(newItemPurchased);
            CPC->GetHUDManager()->GetInventoryHUD()->LoadItems();
            CPC->GetGameMode()->GetQuestManager()->OnItemPickup(newItemPurchased);
            return true;
         }
         else {
            CPC->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NSLOCTEXT("InventoryFull", "NotEnoughSpaceStore", "Not enough space in inventory to purchase item!"));
         }
      }
   }
   return false;
}

bool UStoreInventory::EnoughFunds(int numPurchasing)
{
   // Ensure we have more money than required
   if(itemPrice->money * numPurchasing <= CPC->GetBasePlayer()->money) {
      if(itemPrice->items.Num() > 0) {
         int itemCount = 0;
         // Check we have the items necessary to trade
         for(FMyItem& tradeItems : itemPrice->items) {
            itemCount = interactingHeroPack->FindItemCount(tradeItems.id);
            if(itemCount < tradeItems.count * numPurchasing) {
               CPC->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NotEnoughItemsText);
               return false;
            }
         }
      }
      return true;
   }
   CPC->GetHUDManager()->GetIngameHUD()->DisplayHelpText(NotEnoughMoneyText);
   return false;
}

bool UStoreInventory::OnWidgetAddToViewport_Implementation()
{
   // Load up our shopkeeper when adding the hud
   shopkeeper = Cast<AShopNPC>(CPC->GetBasePlayer()->heroInBlockingInteraction->GetCurrentInteractable());
   if(shopkeeper)
   {
      SetBackPack(shopkeeper->itemsToSellBackpack);
      return Super::OnWidgetAddToViewport_Implementation();
   }
   return false;
}

void UStoreInventory::UseItemAtInventorySlot_Implementation(int32 iSlot)
{
   itemSlot  = iSlot;
   itemToBuy = GetBackpack()->GetItem(itemSlot).id;
   itemPrice = &shopkeeper->GetItemPrice(itemToBuy);

   // Check to make sure the hero doesn't die while talking to the shopkeeper... which could be to poison or to an enemy if enemies spawn in the same room
   if(IsValid(CPC->GetBasePlayer()->heroInBlockingInteraction))
      interactingHeroPack = CPC->GetBasePlayer()->heroInBlockingInteraction->backpack;

   if(!UItemManager::Get().GetItemInfo(itemToBuy)->isStackable) {
      if(EnoughFunds(1))
         hudManagerRef->ShowConfirmationBox("OnItemPurchased", this, confirmTitleText, ensurePurchaseText);
   } else
      hudManagerRef->ShowInputBox("OnItemsPurchased", this, confirmTitleText, ensurePurchaseText);
}
