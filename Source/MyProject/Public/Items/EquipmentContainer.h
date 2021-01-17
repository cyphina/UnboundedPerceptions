#pragma once

#include "Object.h"
#include "EquipmentContainer.generated.h"

class UBackpack;

DECLARE_DELEGATE_TwoParams(FOnEquipmentContainerChanged, int, bool);

using Equip_Slot_Arr = TStaticArray<int, 7>;

/**
 * @brief UObject container for holding equipment data.
 */
UCLASS(NotBlueprintable)
class MYPROJECT_API UEquipmentContainer : public UObject
{
   GENERATED_BODY()

public:
   /** Returns id of the equip at the specified slot */
   int GetEquipAtSlot(int slotIndex) const { return equips[slotIndex]; }

   const Equip_Slot_Arr& GetEquips() const { return equips; }

   int GetWeaponId() const { return equips[5]; }

   /**Modifies the storage container to hold the new item with id equipItem as well as swaps in and out the appropriate stats
    * @param equipItemID - Item ID of the item to equip
    * @return - Returns Item ID of any previously equipped equipment
    */
   int Equip(int equipItemID);

   /** Frees the slot from the container as well as removing stat bonuses */
   void Unequip(int slot);

   FOnEquipmentContainerChanged& OnEquipmentChanged() { return OnEquipmentChangedEvent; }

   using RangedForIteratorType = Equip_Slot_Arr::RangedForIteratorType;
   using RangedForConstIteratorType = Equip_Slot_Arr::RangedForConstIteratorType;

   RangedForIteratorType begin() { return equips.begin(); }

   RangedForConstIteratorType begin() const { return equips.begin(); }

   RangedForIteratorType end() { return equips.end(); }

   RangedForConstIteratorType end() const { return equips.end(); }

private:
   /** Move equip from equipment to some other inventory (storage, hero).  Can only be done through dragging */
   int SwapEquipsFromInventory(int equipID, int equipSlot);

   /** 0 - Head, 1 - Body, 2 - Legs, 3 - Acc1, 4 - Codex, 5-9 - Codex Weapons */
   Equip_Slot_Arr equips;

   FOnEquipmentContainerChanged OnEquipmentChangedEvent;
};
