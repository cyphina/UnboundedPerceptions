// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Weapon.h"
#include "WorldObjects/BaseHero.h"
#include "EquipmentMenu.h"
#include "UserInput.h"
#include "UI/HUDManager.h"


bool UEquipmentMenu::OnWidgetAddToViewport_Implementation()
{
	return Super::OnWidgetAddToViewport_Implementation();
}

void UEquipmentMenu::Equip(UEquip* equip)
{

	if (equip->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Helmet")
	{

	}
	else if (equip->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Body")
	{

	}
	else if (equip->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Legs")
	{

	}
	else if (equip->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Helmet")
	{

	}
	else if (equip->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Codex")
	{

	}
	else if (equip->itemInfo.itemType.GetTagName() == "Item.Equippable.Armor.Weapon")
	{

	}
	else
	{

	}

}

void UEquipmentMenu::Unequip(int slotToUnequip)
{
	hero->UnequipItem(slotToUnequip);
}

void UEquipmentMenu::Update_Implementation()
{

}

void UEquipmentMenu::SwapWeps(UWeapon* weapon)
{
}
