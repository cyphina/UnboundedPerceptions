#include "MyProject.h"
#include "EquipmentMenu.h"
<<<<<<< HEAD
#include "Items/ItemDelegateStore.h"
#include "UI/HUDManager.h"

void UEquipmentMenu::NativeOnInitialized()
{
   ItemChangeEvents::OnEquipmentChangedEvent.AddUObject(this, &UEquipmentMenu::OnEquipmentChanged);
=======

#include "BaseHero.h"
#include "BasePlayer.h"
#include "EquipmentContainer.h"
#include "ItemFunctionLibrary.h"
#include "TextBlock.h"
#include "UserInput.h"
#include "ItemDelegateContext.h"
#include "UI/HUDManager.h"

void UEquipmentMenu::NativeOnInitialized()
{
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnEquipmentChanged().AddUObject(this, &UEquipmentMenu::OnEquipmentChanged);
   
   equipSlots[0] = helmetSlot;
   equipSlots[1] = bodySlot;
   equipSlots[2] = gloveSlot;
   equipSlots[3] = footSlot;
   equipSlots[4] = accessorySlot;
   equipSlots[5] = primaryWeaponSlot;
   equipSlots[6] = offHandSlot;
}

bool UEquipmentMenu::OnWidgetAddToViewport_Implementation()
{
   if(CPC->GetBasePlayer()->GetSelectedHeroes().Num() > 0)
   {
      heroRef = CPC->GetBasePlayer()->GetSelectedHeroes()[0];
      if(heroRef)
      {
         SetupEquipImages();
         Text_MenuTitle->SetText(FText::Format(NSLOCTEXT("EquipMenu", "EquipMenuTitle", "{0}'s Equipment"), heroRef->GetGameName()));
         return true;
      }
   }
   return false;
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
>>>>>>> componentrefactor
}

void UEquipmentMenu::OnEquipmentChanged(const ABaseHero* heroThatChanged, TArray<int> updatedInventorySlots)
{
   if(GetVisibility() != ESlateVisibility::Collapsed) {
      if(GetEquippedHero() == heroThatChanged) SetupEquipImages();
   }
}

void UEquipmentMenu::OnEquipmentChanged(const ABaseHero* heroThatChanged, const FMyItem& changedEquip)
{
   if(GetVisibility() != ESlateVisibility::Collapsed) {
      if(GetEquippedHero() == heroThatChanged) Update();
   }
}
