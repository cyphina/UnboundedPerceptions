#include "MyProject.h"
#include "EquipmentContainer.h"
#include "WorldObjects/BaseHero.h"
#include "ItemManager.h"

int UEquipmentContainer::Equip(int equipItemID)
{
   const FGameplayTag itemType = UItemManager::Get().GetItemInfo(equipItemID)->itemType;

   // Depending on what kind of equip we have, swap item into different slots allocated for that equip

   if(itemType.GetTagName() == "Item.Equippable.Armor.Helmet")
   {
      return SwapEquipsFromInventory(equipItemID, 0);
   }
   if(itemType.GetTagName() == "Item.Equippable.Armor.Body")
   {
      return SwapEquipsFromInventory(equipItemID, 1);
   }
   if(itemType.GetTagName() == "Item.Equippable.Armor.Gloves")
   {
      return SwapEquipsFromInventory(equipItemID, 2);
   }
   if(itemType.GetTagName() == "Item.Equippable.Armor.Foot")
   {
      return SwapEquipsFromInventory(equipItemID, 3);
   }
   if(itemType.GetTagName() == "Item.Equippable.Armor.Accessory")
   {
      return SwapEquipsFromInventory(equipItemID, 4);
   }
   if(itemType.GetTagName() == "Item.Equippable.Weapon")
   {
      return SwapEquipsFromInventory(equipItemID, 5);
   }
   if(itemType.GetTagName() == "Item.Equippable.OffHand")
   {
      return SwapEquipsFromInventory(equipItemID, 6);
   }
   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Error, equipping unknown type of item");
   return 0;
}

int UEquipmentContainer::SwapEquipsFromInventory(int equipID, int equipSlot)
{
   const int prevEquipId = equips[equipSlot];
   if(prevEquipId > 0)
   {
      OnEquipmentChanged().ExecuteIfBound(equips[equipSlot], false);
      equips[equipSlot] = equipID;
      OnEquipmentChanged().Execute(equipID, true);
      return prevEquipId;
   }
   equips[equipSlot] = equipID;
   OnEquipmentChanged().Execute(equipID, true);
   return 0;
}

void UEquipmentContainer::Unequip(int slot)
{
   if(equips[slot] > 0)
   {
      FMyItem item = FMyItem(equips[slot]);
      OnEquipmentChanged().Execute(equips[slot], false);
      equips[slot] = 0;
   }
}
