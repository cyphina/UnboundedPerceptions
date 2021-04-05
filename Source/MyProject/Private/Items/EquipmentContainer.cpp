#include "MyProject.h"
#include "EquipmentContainer.h"
#include "WorldObjects/BaseHero.h"
#include "ItemManager.h"

int UEquipmentContainer::Equip(int equipItemID)
{
   const FGameplayTag itemType = UItemManager::Get().GetItemInfo(equipItemID)->itemType;

   // Depending on what kind of equip we have, swap item into different slots allocated for that equip

<<<<<<< HEAD
   if(itemType.GetTagName() == "Item.Equippable.Armor.Helmet") {
      return SwapEquipsFromInventory(equipItemID, 0);
   } else if(itemType.GetTagName() == "Item.Equippable.Armor.Body") {
      return SwapEquipsFromInventory(equipItemID, 1);
   } else if(itemType.GetTagName() == "Item.Equippable.Armor.Legs") {
      return SwapEquipsFromInventory(equipItemID, 2);
   } else if(itemType.GetTagName() == "Item.Equippable.Armor.Accessory") {
      if(equips[3] < 0)
         return SwapEquipsFromInventory(equipItemID, 3);
      else
         return SwapEquipsFromInventory(equipItemID, 4);
   } else if(itemType.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Equippable.Weapon"))) {
      for(int i = 5; i < 9; i++) {
         // if our equip slots are full, just swap with the last weapon slot
         if(equips[i] <= 0) { return SwapEquipsFromInventory(equipItemID, i); }
      }
      return SwapEquipsFromInventory(equipItemID, 9);
   } else {
      GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Error, equipping unknown type of item");
      return 0;
   }
=======
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
   if(itemType.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Equippable.Weapon")))
   {
      return SwapEquipsFromInventory(equipItemID, 5);
   }
   if(itemType.GetTagName() == "Item.Equippable.OffHand")
   {
      return SwapEquipsFromInventory(equipItemID, 6);
   }
   
   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Error, equipping unknown type of item");
   return 0;
>>>>>>> componentrefactor
}

int UEquipmentContainer::SwapEquipsFromInventory(int equipID, int equipSlot)
{
   const int prevEquipId = equips[equipSlot];
<<<<<<< HEAD
   if(prevEquipId > 0) {
      OnEquipmentChanged().ExecuteIfBound(equips[equipSlot], false);
      equips[equipSlot] = equipID;
      OnEquipmentChanged().Execute(equipID, true);
      return prevEquipId;
   } else {
      equips[equipSlot] = equipID;
      OnEquipmentChanged().Execute(equipID, true);
      return 0;
   }
}

void UEquipmentContainer::SwapEquips(int equipSlot1, int equipSlot2)
{
   if(equipSlot1 != equipSlot2) {
      if(equips[equipSlot1] > 0) {
         // Right now we can only swap accessories (used to be able to do weapons but I decided not to use a crazy weapon system)
         if(UItemManager::Get().GetItemInfo(equips[equipSlot1])->itemType == FGameplayTag::RequestGameplayTag("Item.Equippable.Armor.Acc")) {
            if(equips[equipSlot2] > 0) {
               if(UItemManager::Get().GetItemInfo(equips[equipSlot1])->itemType == UItemManager::Get().GetItemInfo(equips[equipSlot2])->itemType) {
                  const int equipInFirstSlot = equips[equipSlot1];
                  equips[equipSlot1]         = equips[equipSlot2];
                  equips[equipSlot2]         = equipInFirstSlot;
               }
            } else {
               equips[equipSlot2] = equips[equipSlot1];
               equips[equipSlot1] = 0;
            }
         }
      }
   }
=======
   if(prevEquipId > 0)
   {
      OnEquipmentContainerChanged().ExecuteIfBound(equips[equipSlot], false);
      equips[equipSlot] = equipID;
      OnEquipmentContainerChanged().Execute(equipID, true);
      return prevEquipId;
   }
   equips[equipSlot] = equipID;
   OnEquipmentContainerChanged().Execute(equipID, true);
   return 0;
>>>>>>> componentrefactor
}

void UEquipmentContainer::Unequip(int slot)
{
<<<<<<< HEAD
   if(equips[slot] > 0) {
      FMyItem item = FMyItem(equips[slot]);
      OnEquipmentChanged().Execute(equips[slot], false);
=======
   if(equips[slot] > 0)
   {
      FMyItem item = FMyItem(equips[slot]);
      OnEquipmentContainerChanged().Execute(equips[slot], false);
>>>>>>> componentrefactor
      equips[slot] = 0;
   }
}
