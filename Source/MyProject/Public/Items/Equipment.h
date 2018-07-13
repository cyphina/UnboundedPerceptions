#pragma once
#include "CoreMinimal.h"
#include "Weapon.h"
#include "Equipment.generated.h"

/**UObject container for holding equips.  Doesn't manage anything with backpack, adding and removing items from unequipping and equipping items are handled in the UI*/

class UBackpack;

DECLARE_DELEGATE_TwoParams(FOnEquipped, int, bool);

UCLASS(ClassGroup=(Custom))
class MYPROJECT_API UEquipment : public UObject
{
	GENERATED_BODY()
		
	//0 - Head, 1 - Body, 2 - Legs, 3 - Acc1, 4 - Codex, 5-9 - Codex Weapons
	TStaticArray<int,10>								equips;

	/**Move equip from equipment to some other inventory (storage, hero).  Can only be done through dragging*/
	int										SwapEquipsFromInventory(int equipID, int equipSlot);

public:	

	UEquipment();

	FORCEINLINE TArray<int>					GetEquips() const
	{
		TArray<int> equipsCopy;
		for(int i = 0 ; i < 10; ++i)
		{
			equipsCopy.Add(equips[i]);
		}
		return MoveTemp(equipsCopy);
	}

	/**Modifies the storage container to hold the new item with id equipItem as well as swaps in and out the appropriate stats
	 * @param equipItem - ID of the item to equip
	 */
	int										Equip(int equipItem);
	
	/**Frees the slot from the container as well as removing stat bonuses.*/
	void									Unequip(int slot);

	/**Swap pieces of equipment that are in interchangeable slots in the equip menu*/
	void									SwapEquips(int equipSlot1, int equipSlot2);

	FOnEquipped								OnEquipped;
};
