#include "MyProject.h"
#include "EquipmentSlot.h"
#include "UserInput.h"
#include "HUDManager.h"
#include "ItemFunctionLibrary.h"
#include "RTSIngameWidget.h"
#include "ToolTipWidget.h"
#include "WorldObjects/BaseHero.h"
#include "Items/ItemManager.h"
#include "Items/EquipmentMenu.h"
#include "UMG/Public/Components/TextBlock.h"

void UEquipmentSlot::NativeConstruct()
{
   Super::NativeConstruct();
   infoText->SetVisibility(ESlateVisibility::Hidden);
}

void UEquipmentSlot::OnBtnClick()
{
   // Unequip when this slot is clicked
   CPCRef->GetHUDManager()->GetEquipHUD()->GetEquippedHero()->Unequip(slotIndex);
}

void UEquipmentSlot::ShowDesc(UToolTipWidget* tooltip)
{
   // Show information about the equipment in the slot
   int itemId = CPCRef->GetHUDManager()->GetEquipHUD()->GetEquippedHero()->GetEquipment()[slotIndex];
   if(itemId > 0) {
      auto itemInfo = UItemManager::Get().GetItemInfo(itemId);
      if(!itemInfo->name.IsEmpty()) {
         // Convert the rarity enum value to a string
         const UEnum* eRarity = FindObject<UEnum>(ANY_PACKAGE, TEXT("ERarity"), true);
         if(!eRarity)
            return;
         const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
         const FText bonusDesc  = UItemFunctionLibrary::GetBonusDescription(itemId);
         tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, bonusDesc, FText::GetEmpty());
      }
   }
}
