#pragma once

#include "Object.h"
#include "EquipmentContainer.generated.h"

class UBackpack;

DECLARE_DELEGATE_TwoParams(FOnEquipmentContainerChanged, int, bool);

using Equip_Slot_Arr = TStaticArray<int, 10>;

/**
 * @brief UObject container for holding equipment data.
 */
UCLASS(NotBlueprintable)
class MYPROJECT_API UEquipmentContainer : public UObject
{
   GENERATED_BODY()

 public:
   /** Returns id of the equip at the specified slot */
   const int GetEquipAtSlot(int slotIndex) const { return equips[slotIndex]; }

   int GetWeaponId() const { return equips[5]; }

   /**Modifies the storage container to hold the new item with id equipItem as well as swaps in and out the appropriate stats
    * @param equipItemID - Item ID of the item to equip
    * @return - Returns Item ID of any previously equipped equipment
    */
   int Equip(int equipItemID);

   /** Frees the slot from the container as well as removing stat bonuses */
   void Unequip(int slot);

   /** Swap pieces of equipment that are in interchangeable slots in the equip menu */
   void SwapEquips(int equipSlot1, int equipSlot2);

   FOnEquipmentContainerChanged& OnEquipmentChanged() { return OnEquipmentChangedEvent; }

 private:
   /** Move equip from equipment to some other inventory (storage, hero).  Can only be done through dragging */
   int SwapEquipsFromInventory(int equipID, int equipSlot);

   /** 0 - Head, 1 - Body, 2 - Legs, 3 - Acc1, 4 - Codex, 5-9 - Codex Weapons */
   Equip_Slot_Arr equips;

   FOnEquipmentContainerChanged OnEquipmentChangedEvent;
};
