// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "HeroInventory.h"

#include "UserInput.h"
#include "BasePlayer.h"
#include "UI/HUDManager.h"
#include "WorldObjects/BaseHero.h"

#include "Items/EquipmentMenu.h"
#include "Items/Equipment.h"
#include "ItemManager.h"
#include "Items/Consumable.h"
#include "InventoryView.h"
#include "Weapon.h"

#include "Backpack.h"

void UHeroInventory::UseItemAtInventorySlot_Implementation(int32 iSlot)
{
	if (!GetBackpack()->IsEmptySlot(iSlot))
	{
		FMyItem itemUsed = GetBackpack()->GetItem(iSlot);
		FGameplayTag itemType = UItemManager::Get().GetItemInfo(itemUsed.id)->itemType;
		ABaseHero* hero = CPC->GetBasePlayer()->heroes[hIndex];

		if (itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Equippable")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, TEXT("USED AN EQUIP"));
			if (hero)
			{
				hero->Equip(iSlot);
			}
		}

		//if we're using a consumeable
		else if(itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Consumeable"))) 
		{
#if UE_EDITOR
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, TEXT("USED A CONSUMEABLE"));
#endif	
			//set it as the item the hero is going to be using, so we can now target the target which the consumeable is to be used upon
			hero->BeginUseItem(itemUsed.id);
		}
		else //some item type that isnt specified
		{
#if UE_EDITOR
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, TEXT("ERROR WITH ITEM TYPE"));
#endif
		}
	}
}
