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

#include "ItemDelegateStore.h"

const FText UStoreInventory::NotEnoughItemsText       = NSLOCTEXT("HelpMessages", "MisisngItems", "Missing required items for trade");
const FText UStoreInventory::NotEnoughMoneyText       = NSLOCTEXT("HelpMessages", "NotEnoughSqueezies", "You don't have enough squeezies...");
const FText UStoreInventory::confirmTitleText         = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirmTitle", "Purchase");
const FText UStoreInventory::ensurePurchaseText       = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirm", "How many of these would you like to purchase?");
const FText UStoreInventory::ensurePurchaseSingleText = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirm", "Are you sure you want to purchase this?");

bool UStoreInventory::OnItemPurchased() const
{
   interactingHeroPack->RemoveItems(itemPrice->items);
   if(ABasePlayer* basePlayer = CPC->GetBasePlayer()) {
      CPC->GetBasePlayer()->SetMoney(CPC->GetBasePlayer()->GetMoney() - itemPrice->money);
      FMyItem    newItemPurchased{itemToBuy, 1};
      ABaseHero* purchasingHero = CPC->GetBasePlayer()->heroInBlockingInteraction;
      interactingHeroPack->AddItem(newItemPurchased);

      CPC->GetWidgetProvider()->GetIngameHUD()->GetInventoryHUD()->LoadItems();
      ItemChangeEvents::OnItemPurchasedEvent.Broadcast(purchasingHero, ) CPC->GetGameMode()->GetQuestManager()->OnItemPickedUp(newItemPurchased);
      return true;
   }
   return false;
}

bool UStoreInventory::OnItemsPurchased(FString howManyItems)
{
   if(howManyItems != "") {
      // Reads in number of items purchased from confirmation box
      const int num = FCString::Atoi(*howManyItems);
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
            CPC->GetBasePlayer()->SetMoney(CPC->GetBasePlayer()->GetMoney() - itemPrice->money * num);
            FMyItem newItemPurchased{itemToBuy, num};
            interactingHeroPack->AddItem(newItemPurchased);
            ItemChangeEvents::OnItemPurchasedEvent.Broadcast(purchasingHero, ) CPC->GetGameMode()->GetQuestManager()->OnItemPickedUp(newItemPurchased);

            CPC->GetGameMode()->GetQuestManager()->OnItemPickedUp(newItemPurchased);
            return true;
         } else {
            URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("InventoryFull", "NotEnoughSpaceStore", "Not enough space in inventory to purchase item!"));
         }
      }
   }
   return false;
}

bool UStoreInventory::EnoughFunds(int numPurchasing) const
{
   if(itemPrice->money * numPurchasing <= CPC->GetBasePlayer()->money) {
      if(itemPrice->items.Num() > 0) {
         int itemTradingInCount = 0;
         for(const FMyItem& tradeItems : itemPrice->items) {
            itemTradingInCount = interactingHeroPack->FindItemCount(tradeItems.id);
            if(itemTradingInCount < tradeItems.count * numPurchasing) {
               URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NotEnoughItemsText);
               return false;
            }
         }
      }
      return true;
   }
   CPC->GetWidgetProvider()->GetIngameHUD()->DisplayHelpText(NotEnoughMoneyText);
   return false;
}

bool UStoreInventory::OnWidgetAddToViewport_Implementation()
{
   if(shopkeeper = Cast<AShopNPC>(CPC->GetBasePlayer()->heroInBlockingInteraction->GetCurrentInteractable())) {
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

   if(IsValid(CPC->GetBasePlayer()->heroInBlockingInteraction)) {
      interactingHeroPack = CPC->GetBasePlayer()->heroInBlockingInteraction->backpack;

      if(!UItemManager::Get().GetItemInfo(itemToBuy)->isStackable) {
         if(EnoughFunds(1)) hudManagerRef->ShowConfirmationBox("OnItemPurchased", this, confirmTitleText, ensurePurchaseText);
      } else
         hudManagerRef->ShowInputBox("OnItemsPurchased", this, confirmTitleText, ensurePurchaseSingleText);
   }
}
