// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Weapon.h"

UWeapon::UWeapon()
{

}

UWeapon::~UWeapon()
{

}

void UWeapon::SetupBaseValues()
{
	if (itemInfo.itemType.GetTagName() == "Item.Weapon.Axe")
	{
		weaponPower *= 2; 
		baseAttackTime = 1.5;
	}
	else if (itemInfo.itemType.GetTagName() == "Item.Weapon.Bow")
	{
		weaponPower *= 1.5;
		baseAttackTime = 1.2;
	}
	else if (itemInfo.itemType.GetTagName() == "Item.Weapon.Sword")
	{
		weaponPower *= 1.3;
		baseAttackTime = 1.05;
	}
	else if (itemInfo.itemType.GetTagName() == "Item.Weapon.Wand")
	{
		weaponPower *= 1.1;
		baseAttackTime = 1.2;
	}	
	else
	{
		//do nothing
	}
}
