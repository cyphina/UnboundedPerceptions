// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Equipment.h"
#include "WorldObjects/BaseHero.h"
#include "ItemManager.h"


// Sets default values for this component's properties
UEquipment::UEquipment()
{
	equips.SetNum(10);
}

void UEquipment::Equip(int equipItem)
{
	FEquipLookupRow* e = UItemManager::Get().GetEquipInfo(equipItem);
	FGameplayTag itemType = UItemManager::Get().GetItemInfo(equipItem)->itemType;

	//Depending on what kind of equip we have, swap item into different slots allocated for that equip
	if (itemType.GetTagName() == "Item.Equippable.Armor.Helmet")
	{
		SwapEquipsFromInventory(equipItem, 0);
	}
	else if (itemType.GetTagName() == "Item.Equippable.Armor.Body")
	{
		SwapEquipsFromInventory(equipItem, 1);
	}
	else if (itemType.GetTagName() == "Item.Equippable.Armor.Legs")
	{
		SwapEquipsFromInventory(equipItem, 2);
	}
	else if (itemType.GetTagName() == "Item.Equippable.Armor.Accessory")
	{
		if (equips[3] < 0)
			SwapEquipsFromInventory(equipItem, 3);
		else
			SwapEquipsFromInventory(equipItem, 4);
	}
	else if (itemType.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Equippable.Weapon")))
	{
		for(int i = 5; i < 9; i++)
		{
			if(equips[i] <= 0)
			{
				SwapEquipsFromInventory(equipItem, i);
				return;
			}
		} //if our equip slots are full, just swap with the last equip
		SwapEquipsFromInventory(equipItem, 9);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Error, equipping unknown type of item");
	}
}

void UEquipment::SwapEquipsFromInventory(int equipID, int equipSlot)
{
	if (equips[equipSlot] > 0) //already equipped item in slot
	{
		FMyItem item = FMyItem(equipID);
		//if we sucessfully add our item that is equipped to the inventory
		if (heroRef->backpack->AddItem(item))
		{
			//remove bonuses and replace equipment
			OnEquipped.Execute(equips[equipSlot], false);
			equips[equipSlot] = equipID;
			OnEquipped.Execute(equipID, true);
		}
	}
	else //no item in slot currently
	{
		//just remove item and set bonuses
		equips[equipSlot] = equipID;
		OnEquipped.Execute(equipID, true);
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

void UEquipment::UnequipItem(int slot)
{
	if (equips[slot] > 0 && heroRef->backpack->Count() < heroRef->backpack->GetItemMax())
	{
		FMyItem item = FMyItem(equips[slot]);
		heroRef->backpack->AddItem(item);
		OnEquipped.Execute(equips[slot], false);
		equips[slot] = 0;
	}
}



