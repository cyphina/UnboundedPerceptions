// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StoreInventory.h"
#include "UserInput.h"
#include "UI/HUDManager.h"

#include "QuestManager.h"

#include "UI/UserWidgets/RTSIngameWidget.h"
#include "Items/HeroInventory.h"

#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/ShopNPC.h"
#include "ItemManager.h"

#include "ItemDelegateContext.h"

const FText UStoreInventory::NotEnoughItemsText       = NSLOCTEXT("HelpMessages", "MisisngItems", "Missing required items for trade");
const FText UStoreInventory::NotEnoughMoneyText       = NSLOCTEXT("HelpMessages", "NotEnoughSqueezies", "You don't have enough squeezies...");
const FText UStoreInventory::confirmTitleText         = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirmTitle", "Purchase");
const FText UStoreInventory::ensurePurchaseText       = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirm", "How many of these would you like to purchase?");
const FText UStoreInventory::ensurePurchaseSingleText = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirm", "Are you sure you want to purchase this?");

UStoreInventory::UStoreInventory() : interactingHero()
{
   
}

bool UStoreInventory::OnItemPurchased() const
{
   GetInteractingHeroBackpack()->RemoveItems(itemPrice->tradeItems);
   if(ABasePlayer* basePlayer = CPC->GetBasePlayer()) {
      CPC->GetBasePlayer()->SetMoney(CPC->GetBasePlayer()->GetMoney() - itemPrice->money);
      FMyItem    newItemPurchased{itemToBuy, 1};
      ABaseHero* purchasingHero = CPC->GetBasePlayer()->heroInBlockingInteraction;
      GetInteractingHeroBackpack()->AddItem(newItemPurchased);

      CPC->GetWidgetProvider()->GetIngameHUD()->GetInventoryHUD()->LoadItems();
      CPC->GetLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemPurchased().Broadcast(purchasingHero, newItemPurchased);
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

         FMyItem newItemPurchased{itemToBuy, num};
         if(GetInteractingHeroBackpack()->AddItem(newItemPurchased)) {
            ABaseHero* purchasingHero = CPC->GetBasePlayer()->heroInBlockingInteraction;
            CPC->GetLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemPurchased().Broadcast(purchasingHero, newItemPurchased);
            return true;
         } else {
            URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("InventoryFull", "NotEnoughSpaceStore", "Not enough space in inventory to purchase item!"));
         }
         
         TArray<FMyItem> itemPriceItems = itemPrice->tradeItems;
         for(FMyItem& item : itemPriceItems) {
            item.count *= num;
         }

         GetInteractingHeroBackpack()->RemoveItems(itemPriceItems);
         CPC->GetBasePlayer()->SetMoney(CPC->GetBasePlayer()->GetMoney() - itemPrice->money * num);
      }
   }
   return false;
}

bool UStoreInventory::EnoughFunds(int numPurchasing) const
{
   if(itemPrice->money * numPurchasing <= CPC->GetBasePlayer()->GetMoney()) {
      if(itemPrice->tradeItems.Num() > 0) {
         int itemTradingInCount = 0;
         for(const FMyItem& tradeItems : itemPrice->tradeItems) {
            itemTradingInCount = GetInteractingHeroBackpack()->FindItemCount(tradeItems.id);
            if(itemTradingInCount < tradeItems.count * numPurchasing) {
               URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NotEnoughItemsText);
               return false;
            }
         }
      }
      return true;
   }
   URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NotEnoughMoneyText);
   return false;
}

bool UStoreInventory::OnWidgetAddToViewport_Implementation()
{
   shopkeeper = Cast<AShopNPC>(CPC->GetBasePlayer()->heroInBlockingInteraction->GetCurrentInteractable());
   if(shopkeeper) {
      SetBackPack(shopkeeper->itemsToSellBackpack);
      return Super::OnWidgetAddToViewport_Implementation();
   }
   return false;
}

UBackpack* const UStoreInventory::GetInteractingHeroBackpack() const
{
   if(IsValid(*interactingHero))
   {
      return &(*interactingHero)->GetBackpack();
   }
   return nullptr;
}

void UStoreInventory::UseItemAtInventorySlot_Implementation(int32 iSlot)
{
   itemSlot  = iSlot;
   itemToBuy = GetBackpack()->GetItem(itemSlot).id;
   itemPrice = &shopkeeper->GetItemPrice(itemToBuy);

   if(IsValid(CPC->GetBasePlayer()->heroInBlockingInteraction)) {

      if(!UItemManager::Get().GetItemInfo(itemToBuy)->isStackable) {
         if(EnoughFunds(1)) hudManagerRef->ShowConfirmationBox("OnItemPurchased", this, confirmTitleText, ensurePurchaseText);
      } else
         hudManagerRef->ShowInputBox("OnItemsPurchased", this, confirmTitleText, ensurePurchaseSingleText);
   }
}

void UStoreInventory::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   ABaseHero* interactingHeroLoc = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())->GetBasePlayer()->heroInBlockingInteraction;
   interactingHero = &interactingHeroLoc;
}

