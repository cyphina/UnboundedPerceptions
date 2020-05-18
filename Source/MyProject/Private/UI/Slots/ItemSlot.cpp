#include "MyProject.h"
#include "ItemSlot.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "Items/Inventory.h"
#include "Items/InventoryView.h"
#include "ItemFunctionLibrary.h"
#include "ToolTipWidget.h"
#include "Items/ItemManager.h"
#include "Items/Backpack.h"

void UItemSlot::OnBtnClick()
{
   int backpackIndex = inventoryRef->inventoryView->GetCorrespondingBackpackIndex(slotIndex);
   int itemId        = inventoryRef->GetBackpack()->GetItem(backpackIndex).id;
   if(itemId > 0)
      inventoryRef->UseItemAtInventorySlot(backpackIndex);
}

void UItemSlot::ShowDesc(UToolTipWidget* tooltip)
{
   int backpackIndex = inventoryRef->inventoryView->GetCorrespondingBackpackIndex(slotIndex);
   if(inventoryRef->GetBackpack()->IsEmptySlot(backpackIndex))
      return;
   int itemId = inventoryRef->GetBackpack()->GetItem(backpackIndex).id;

   if(itemId > 0) {
      // If this item is an equippable
      auto itemInfo = UItemManager::Get().GetItemInfo(itemId);
      if(itemInfo->itemType.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Equippable", false))) {
         const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
         const FText bonusDesc  = UItemFunctionLibrary::GetBonusDescription(itemId);
         tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, bonusDesc, FText::GetEmpty());
      }
      // Else we don't have a piece of equipment
      else {
         const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
         tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, FText::GetEmpty(), FText::GetEmpty());
      }
   }
}
