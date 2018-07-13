// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Equipment.h"
#include "WorldObjects/BaseHero.h"
#include "ItemManager.h"


// Sets default values for this component's properties
UEquipment::UEquipment()
{

}

int UEquipment::Equip(int equipItem)
{
	FEquipLookupRow* e = UItemManager::Get().GetEquipInfo(equipItem);
	FGameplayTag itemType = UItemManager::Get().GetItemInfo(equipItem)->itemType;

	//Depending on what kind of equip we have, swap item into different slots allocated for that equip
	if (itemType.GetTagName() == "Item.Equippable.Armor.Helmet")
	{
		return SwapEquipsFromInventory(equipItem, 0);
	}
	else if (itemType.GetTagName() == "Item.Equippable.Armor.Body")
	{
		return SwapEquipsFromInventory(equipItem, 1);
	}
	else if (itemType.GetTagName() == "Item.Equippable.Armor.Legs")
	{
		return SwapEquipsFromInventory(equipItem, 2);
	}
	else if (itemType.GetTagName() == "Item.Equippable.Armor.Accessory")
	{
		if (equips[3] < 0)
			return SwapEquipsFromInventory(equipItem, 3);
		else
			return SwapEquipsFromInventory(equipItem, 4);
	}
	else if (itemType.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Equippable.Weapon")))
	{
		for(int i = 5; i < 9; i++)
		{
			if(equips[i] <= 0)
			{
				return SwapEquipsFromInventory(equipItem, i);
			}
		} //if our equip slots are full, just swap with the last equip
		return SwapEquipsFromInventory(equipItem, 9);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Error, equipping unknown type of item");
		return 0;
	}
}

int UEquipment::SwapEquipsFromInventory(int equipID, int equipSlot)
{
	int prevEquipId = equips[equipSlot];
	if (prevEquipId > 0) //already equipped item in slot
	{
		//remove bonuses and replace equipment
		OnEquipped.Execute(equips[equipSlot], false);
		equips[equipSlot] = equipID;
		OnEquipped.Execute(equipID, true);
		return prevEquipId;
	}
	else //no item in slot currently
	{
		//just remove item and set bonuses
		equips[equipSlot] = equipID;
		OnEquipped.Execute(equipID, true);
		return 0;
	}
	 
}

void UEquipment::SwapEquips(int equipSlot1, int equipSlot2)
{
	if (equipSlot1 != equipSlot2)
	{
		if (equips[equipSlot1] > 0)
		{
			if (UItemManager::Get().GetItemInfo(equips[equipSlot1])->itemType == UItemManager::Get().GetItemInfo(equips[equipSlot2])->itemType)
			{
				int equipInFirstSlot = equips[equipSlot1];
				equips[equipSlot1] = equips[equipSlot2];
				equips[equipSlot2] = equipInFirstSlot;
			}
		}
	}
}

void UEquipment::Unequip(int slot)
{
	if (equips[slot] > 0)
	{
		FMyItem item = FMyItem(equips[slot]);
		OnEquipped.Execute(equips[slot], false);
		equips[slot] = 0;
	}
}



