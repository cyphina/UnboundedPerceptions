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
   inventoryRef->UseItem(slotIndex);
}

void UItemSlot::ShowDesc(UToolTipWidget* tooltip)
{
   const FMyItem item = inventoryRef->GetBackpackItemAtSlot(slotIndex);
   if(item) {
      const auto itemInfo = UItemManager::Get().GetItemInfo(item.id);
      if(itemInfo->itemType.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Equippable", false))) {
         const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
         const FText bonusDesc  = UItemFunctionLibrary::GetBonusDescription(item.id);
         tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, bonusDesc, FText::GetEmpty());
      }
      // Else we don't have a piece of equipment
      else {
         const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
         tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, FText::GetEmpty(), FText::GetEmpty());
      }
   }
}
