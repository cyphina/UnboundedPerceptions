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

void UShopSlot::OnBtnClick()
{
   auto widgetRef         = CPCRef->GetHUDManager()->GetShopHUD();
   int  adjustedSlotIndex = CPCRef->GetHUDManager()->GetShopHUD()->inventoryView->GetCorrespondingBackpackIndex(slotIndex);
   int  itemId            = widgetRef->GetBackpack()->GetItem(adjustedSlotIndex).id;

   // Buy the item
   if(itemId > 0) {
      widgetRef->UseItemAtInventorySlot(adjustedSlotIndex);
   }
}

void UShopSlot::ShowDesc(UToolTipWidget* tooltip)
{
   const auto widgetRef         = CPCRef->GetHUDManager()->GetShopHUD();
   const int  adjustedSlotIndex = CPCRef->GetHUDManager()->GetShopHUD()->inventoryView->GetCorrespondingBackpackIndex(slotIndex);
   const int  itemId            = widgetRef->GetBackpack()->GetItem(adjustedSlotIndex).id;

   if(itemId > 0) {
      // If this item is an equippable
      auto  itemInfo  = UItemManager::Get().GetItemInfo(itemId);
      FText priceText = MakeItemPriceText(widgetRef->GetShopkeeper()->GetItemPrice(itemId));

      if(itemInfo->itemType.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Equippable", false))) {
         const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
         const FText bonusDesc  = UItemFunctionLibrary::GetBonusDescription(itemId);
         tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, bonusDesc, priceText);
      }
      // Else we don't have a piece of equipment
      else {
         const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
         tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, priceText, FText::GetEmpty());
      }
   }
}

FText UShopSlot::MakeItemPriceText(const FItemPrice& price) const
{
   if(price.items.Num() == 0 && price.money <= 0) {
      FText priceText = NSLOCTEXT("Shop", "FreeItemPrice", "IT'S FREE!!!");
      return priceText;
   }

   FText priceText = NSLOCTEXT("Shop", "ItemPrice", "Price:");
   // List the items we need to trade in for this item
   if(price.items.Num() > 0) {
      for(auto& itemToTrade : price.items) {
         auto itemToTradeInfo = UItemManager::Get().GetItemInfo(itemToTrade.id);
         priceText            = FText::Format(NSLOCTEXT("Shop", "ItemPrice2", "{0}\r\n{1} - {2}"), priceText, itemToTrade.count, itemToTradeInfo->name);
      }
   }
   // List any money we need to trade
   if(price.money > 0) {
      priceText = FText::Format(NSLOCTEXT("Shop", "ItemPrice3", "{0}\r\n{1} Squeezies"), priceText, price.money);     
   }

   return priceText;
}
