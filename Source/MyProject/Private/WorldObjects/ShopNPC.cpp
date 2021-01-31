#include "MyProject.h"
#include "ShopNPC.h"
#include "AllOf.h"
#include "BaseHero.h"
#include "BasePlayer.h"
#include "ItemDelegateContext.h"
#include "ItemManager.h"
#include "UserInput.h"
#include "Items/Backpack.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogUI.h"
#include "RTSIngameWidget.h"
#include "StoreInventory.h"

const FText AShopNPC::NotEnoughItemsText       = NSLOCTEXT("HelpMessages", "MisisngItems", "Missing required items for trade");
const FText AShopNPC::NotEnoughMoneyText       = NSLOCTEXT("HelpMessages", "NotEnoughSqueezies", "You don't have enough squeezies...");
const FText AShopNPC::confirmTitleText         = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirmTitle", "Purchase");
const FText AShopNPC::ensurePurchaseText       = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirm", "How many of these would you like to purchase?");
const FText AShopNPC::ensurePurchaseSingleText = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirm", "Are you sure you want to purchase this?");

void AShopNPC::BeginPlay()
{
   Super::BeginPlay();
   itemsToSellBackpack = UBackpack::CreateBackpack(this, 20);

   for(FMyItem& item : itemsToSell)
   {
      itemsToSellBackpack->AddItem(item);
   }
   controllerRef->GetHUDManager()->GetIngameHUD()->GetShopHUD()->OnSlotSelected().AddUObject(this, &AShopNPC::OnAskToPurchaseItem);
}

void AShopNPC::OnAskToPurchaseItem(int purchaseItemSlotIndex)
{
   const FMyItem itemToBuy = GetItemsToSell()->GetItem(purchaseItemSlotIndex);

   if(IsValid(controllerRef->GetBasePlayer()->heroInBlockingInteraction))
   {
      const FItemPrice* itemPrice = GetItemPrice(itemToBuy.id);
      if(itemPrice)
      {
         if(!UItemManager::Get().GetItemInfo(itemToBuy.id)->isStackable)
         {
            if(EnoughFunds(*itemPrice, 1))
            {
               controllerRef->GetHUDManager()->ShowConfirmationBox(
               FOnConfirmation::CreateWeakLambda(this, [this, itemToBuy, itemPrice]() { return OnItemPurchased(itemToBuy, *itemPrice); }), confirmTitleText,
               ensurePurchaseSingleText);
            }
         } else
         {
            controllerRef->GetHUDManager()->ShowInputBox(
            FOnInputConfirmed::CreateWeakLambda(this, [this, itemToBuy, itemPrice](FString howManyItemsPurchased)
            {
               return OnItemsPurchased(itemToBuy, *itemPrice, howManyItemsPurchased);
            }),
            confirmTitleText, ensurePurchaseText);
         }
      } else
      {
         UE_LOG(LogTemp, Warning, TEXT("Trying to buy an item that has no error price set!"));
      }
   }
}

bool AShopNPC::EnoughFunds(const FItemPrice& itemPrice, int numPurchasing) const
{
   if(itemPrice.money * numPurchasing <= controllerRef->GetBasePlayer()->GetMoney())
   {
      if(itemPrice.tradeItems.Num() > 0)
      {
         int itemTradingInCount = 0;
         for(const FMyItem& tradeItems : itemPrice.tradeItems)
         {
            itemTradingInCount = GetInteractingHeroBackpack()->FindItemCount(tradeItems.id);
            if(itemTradingInCount < tradeItems.count * numPurchasing)
            {
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

UBackpack* AShopNPC::GetInteractingHeroBackpack() const
{
   ABaseHero* interactingHero = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())->GetBasePlayer()->heroInBlockingInteraction;
   if(IsValid(interactingHero))
   {
      return &interactingHero->GetBackpack();
   }
   return nullptr;
}

const FItemPrice& AShopNPC::BP_GetItemPrice(int itemID) const
{
   const FItemPrice* itemPrice = GetItemPrice(itemID);
   check(itemPrice)
   return *const_cast<FItemPrice*>(itemPrice);
}

const FItemPrice* AShopNPC::GetItemPrice(int itemID) const
{
   if(itemPrices.Contains(itemID))
   {
      return &itemPrices[itemID];
   }
   return nullptr;
}

bool AShopNPC::OnItemPurchased(const FMyItem itemPurchased, const FItemPrice& itemPrice) const
{
   if(ABasePlayer* basePlayer = controllerRef->GetBasePlayer())
   {
      basePlayer->SetMoney(basePlayer->GetMoney() - itemPrice.money);
      const FBackpackUpdateResult         itemGainedUpdate  = GetInteractingHeroBackpack()->AddItem(itemPurchased);
      const TArray<FBackpackUpdateResult> itemsTradedUpdate = GetInteractingHeroBackpack()->RemoveItems(itemPrice.tradeItems);

      ABaseHero* purchasingHero = controllerRef->GetBasePlayer()->heroInBlockingInteraction;

      if(UItemDelegateContext* itemDelegateContext = controllerRef->GetLocalPlayer()->GetSubsystem<UItemDelegateContext>())
      {
         itemDelegateContext->OnItemPurchased().Broadcast(purchasingHero, itemGainedUpdate, itemsTradedUpdate);
      }
      return true;
   }
   return false;
}

bool AShopNPC::OnItemsPurchased(const FMyItem itemToBuy, const FItemPrice& itemPrice, FString howManyItems)
{
   if(howManyItems != "" && howManyItems.IsNumeric())
   {
      if(ABasePlayer* basePlayer = controllerRef->GetBasePlayer())
      {
         // Reads in number of items purchased from confirmation box
         const int numberPurchased = FCString::Atoi(*howManyItems);

         if(EnoughFunds(itemPrice, numberPurchased))
         {
            const FBackpackUpdateResult itemBuyUpdateResult = GetInteractingHeroBackpack()->AddItem(FMyItem{itemToBuy.id, numberPurchased});

            if(!itemBuyUpdateResult)
            {
               URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("InventoryFull", "NotEnoughSpaceStore",
                                                                             "Not enough space in inventory to purchase item(s). Purchased as many items as possible!"));
            }

            const TArray<FBackpackUpdateResult> itemPayPackUpdateResult = PayItemReqsForEachItemPurchased(itemPrice, numberPurchased);

            if(Algo::AllOf(itemPayPackUpdateResult))
            {
               ABaseHero* purchasingHero = basePlayer->heroInBlockingInteraction;
               controllerRef->GetLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemPurchased().Broadcast(
               purchasingHero, itemBuyUpdateResult, itemPayPackUpdateResult);
            } else
            {
               UE_LOG(LogTemp, Error, TEXT("Error paying for %d item(s) %d"), itemToBuy.count, itemToBuy.id);
            }

            basePlayer->SetMoney(basePlayer->GetMoney() - itemPrice.money * numberPurchased);
         }
      }
   }
   return false;
}

TArray<FBackpackUpdateResult> AShopNPC::PayItemReqsForEachItemPurchased(const FItemPrice& itemPrice, const int numberPurchased) const
{
   TArray<FMyItem> paymentItems = itemPrice.tradeItems;
   for(FMyItem& item : paymentItems)
   {
      item.count *= numberPurchased;
   }

   return GetInteractingHeroBackpack()->RemoveItems(paymentItems);
}

void AShopNPC::SetupAppropriateView()
{
   controllerRef->GetWidgetProvider()->GetIngameHUD()->GetSocialWindow()->SetShopView();
}
