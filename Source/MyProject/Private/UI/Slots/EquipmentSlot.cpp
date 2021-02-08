#include "MyProject.h"
#include "EquipmentSlot.h"

#include "EquipmentContainer.h"
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

void UEquipmentSlot::ShowDesc(UToolTipWidget* tooltip)
{
   // Show information about the equipment in the slot
   if(AUserInput* CPCRef = Cast<AUserInput>(GetOwningPlayer<AUserInput>()))
   {
      const int itemId = CPCRef->GetWidgetProvider()->GetIngameHUD()->GetEquipHUD()->GetEquippedHero()->GetEquipment()->GetEquipAtSlot(slotIndex);
      if(itemId > 0)
      {
         const auto itemInfo = UItemManager::Get().GetItemInfo(itemId);
         if(!itemInfo->name.IsEmpty())
         {
            // Convert the rarity enum value to a string
            const UEnum* eRarity = FindObject<UEnum>(ANY_PACKAGE, TEXT("ERarity"), true);
            if(!eRarity) return;
            const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
            const FText bonusDesc  = UItemFunctionLibrary::GetBonusDescription(itemId);
            tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, bonusDesc, FText::GetEmpty());
         }
      }
   }
}
