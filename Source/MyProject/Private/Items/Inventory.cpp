// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Backpack.h"
#include "Inventory.h"
#include "Weapon.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "Items/EquipmentMenu.h"
#include "Items/Consumable.h"
#include "InventoryView.h"
#include "WorldObjects/BaseHero.h"

void UInventory::Construct_Implementation()
{
	Super::Construct_Implementation();
}

bool UInventory::OnWidgetAddToViewport_Implementation()
{
	//We still need to fix this to go before we add it to the hud
	Super::OnWidgetAddToViewport_Implementation();

	if (!backpack)
		return false;
	return true;
}

void UInventory::UseItemAtInventorySlot_Implementation(ABaseHero* hero, int32 iSlot)
{
	if (UMyItem* itemUsed = backpack->GetItem(iSlot))
	{
		if (itemUsed->itemInfo.itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Equippable")))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, TEXT("USED AN EQUIP"));
			if (hero)
			{
				UEquip* e = Cast<UEquip>(itemUsed);
				hero->backpack->RemoveItemAtSlot(iSlot); //remove item since it's going to be used
				hero->EquipItem(e);
				CPC->GetHUDManager()->GetEquipHUD()->Update();
			}
		}
		//if we're using a consumeable
		else if(itemUsed->itemInfo.itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Consumeable"))) 
		{
#if UE_EDITOR
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, TEXT("USED A CONSUMEABLE"));
#endif	
			//set it as the item the hero is going to be using, so we can now target the target which the consumeable is to be used upon
			hero->BeginUseItem(Cast<UConsumable>(itemUsed));
			if(itemUsed->itemInfo.itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Consumeable.Restore"))) //one-time use
				hero->backpack->RemoveItemAtSlot(iSlot);
		}
		else //some item type that isnt specified
		{
#if UE_EDITOR
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Magenta, TEXT("ERROR WITH ITEM TYPE"));
#endif
		}
	}
}

void UInventory::SwapItems(UBackpack* otherBackpack, int32 iSlot, int32 iSlot2)
{
	backpack->SwapItems(otherBackpack, iSlot, iSlot2);
}

void UInventory::SwapInventoryActors(AActor* actorWInventory)
{
	if(actorWInventory)
		SetBackPack(Cast<UBackpack>(actorWInventory->GetComponentByClass(UBackpack::StaticClass())));
}



