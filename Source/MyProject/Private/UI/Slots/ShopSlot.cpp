#include "MyProject.h"
#include "ShopSlot.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "InventoryView.h"
#include "ItemFunctionLibrary.h"
#include "RTSIngameWidget.h"
#include "ShopNPC.h"
#include "StoreInventory.h"
#include "ToolTipWidget.h"
#include "WorldObjects/BaseHero.h"
#include "Items/ItemManager.h"

<<<<<<< HEAD
void UShopSlot::OnBtnClick()
{
   auto widgetRef         = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetShopHUD();
   int  adjustedSlotIndex = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetShopHUD()->GetInventoryView()->GetCorrespondingBackpackIndex(slotIndex);
   int  itemId            = widgetRef->GetBackpack()->GetItem(adjustedSlotIndex).id;

   // Buy the item
   if(itemId > 0) {
      widgetRef->UseItemAtInventorySlot(adjustedSlotIndex);
   }
}

void UShopSlot::ShowDesc(UToolTipWidget* tooltip)
{
   const auto widgetRef         = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetShopHUD();
   const int  adjustedSlotIndex = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetShopHUD()->GetInventoryView()->GetCorrespondingBackpackIndex(slotIndex);
   const int  itemId            = widgetRef->GetBackpack()->GetItem(adjustedSlotIndex).id;
=======
void UShopSlot::ShowDesc(UToolTipWidget* tooltip)
{
   if(AUserInput* CPCRef = Cast<AUserInput>(GetOwningPlayer<AUserInput>()))
   {
      const UStoreInventory* shopWidgetRef = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetShopHUD();
      const FMyItem          itemHovered   = shopWidgetRef->GetBackpackItemAtSlot(slotIndex);
>>>>>>> componentrefactor

      if(itemHovered)
      {
         const auto itemInfo = UItemManager::Get().GetItemInfo(itemHovered.id);
         if(const FItemPrice* itemPrice = shopWidgetRef->GetShopkeeper()->GetItemPrice(itemHovered.id))
         {
            const FText priceText = MakeItemPriceText(*itemPrice);

            if(itemInfo->itemType.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Equippable", false)))
            {
               const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
               const FText bonusDesc  = UItemFunctionLibrary::GetBonusDescription(itemHovered.id);
               tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, bonusDesc, priceText);
            }
            else
            {
               const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
               tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, priceText, FText::GetEmpty());
            }
         }
      }
   }
}

FText UShopSlot::MakeItemPriceText(const FItemPrice& price) const
{
   if(price.tradeItems.Num() == 0 && price.money <= 0)
   {
      FText priceText = NSLOCTEXT("Shop", "FreeItemPrice", "IT'S FREE!!!");
      return priceText;
   }

   FText priceText = NSLOCTEXT("Shop", "ItemPrice", "Price:");
   if(price.tradeItems.Num() > 0)
   {
      for(auto& itemToTrade : price.tradeItems)
      {
         const FItemLookupRow* itemToTradeInfo = UItemManager::Get().GetItemInfo(itemToTrade.id);
         priceText            = FText::Format(NSLOCTEXT("Shop", "ItemPrice2", "{0}\r\n{1} - {2}"), priceText, itemToTrade.count, itemToTradeInfo->name);
      }
   }

   if(price.money > 0)
   {
      priceText = FText::Format(NSLOCTEXT("Shop", "ItemPrice3", "{0}\r\n{1} Squeezies"), priceText, price.money);
   }

   return priceText;
}
