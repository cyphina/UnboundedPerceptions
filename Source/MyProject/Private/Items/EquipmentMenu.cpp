#include "MyProject.h"
#include "EquipmentMenu.h"

#include "BaseHero.h"
#include "EquipmentContainer.h"
#include "ItemFunctionLibrary.h"
#include "Items/ItemDelegateStore.h"
#include "UI/HUDManager.h"

void UEquipmentMenu::NativeOnInitialized()
{
   ItemChangeEvents::OnEquipmentChangedEvent.AddUObject(this, &UEquipmentMenu::OnEquipmentChanged);
   equipSlots[0] = helmetSlot;
   equipSlots[1] = bodySlot;
   equipSlots[2] = gloveSlot;
   equipSlots[3] = footSlot;
   equipSlots[4] = accessorySlot;
   equipSlots[5] = primaryWeaponSlot;
   equipSlots[6] = offHandSlot;
}

void UEquipmentMenu::Update_Implementation()
{
}

void UEquipmentMenu::SetupEquipImages()
{
   if(IsValid(GetEquippedHero()))
   {
      int index = 0;
      for(int equipId : *GetEquippedHero()->GetEquipment())
      {
         if(equipId > 0) {
            equipSlots[index]->SetSlotImage(UItemFunctionLibrary::GetItemInfo(equipId).image);
         }
         else
         {
            equipSlots[index]->SetSlotImage(nullptr);
         }
         ++index;
      }
   }
}

void UEquipmentMenu::OnEquipmentChanged(const ABaseHero* heroThatChanged, const FMyItem& changedEquip)
{
   if(GetVisibility() != ESlateVisibility::Collapsed) {
      if(GetEquippedHero() == heroThatChanged) Update();
   }
}
