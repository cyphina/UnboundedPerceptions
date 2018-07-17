// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StoreInventory.h"
#include "UserInput.h"

#include "RTSGameMode.h"
#include "QuestManager.h"

#include "UI/HUDManager.h"
#include "UI/UserWidgets/MainWidget.h"
#include "UI/UserWidgets/MainWidget.h"
#include "Items/HeroInventory.h"

#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/ShopNPC.h"

FText UStoreInventory::NotEnoughItemsText = NSLOCTEXT("HelpMessages", "Items", "Missing required items for trade");
FText UStoreInventory::NotEnoughMoneyText = NSLOCTEXT("HelpMessages", "Items", "You don't have enough squeezies...");

void UStoreInventory::UseItemAtInventorySlot_Implementation(int32 iSlot)
{
	FMyItem itemToBuy = GetBackpack()->GetItem(iSlot);
	FItemPrice itemPrice = shopkeeper->GetItemPrice(itemToBuy.id);
	UBackpack* interactingHeroPack = CPC->GetBasePlayer()->interactedHero->backpack;

	if (itemPrice.money <= CPC->GetBasePlayer()->money)
	{
		if (itemPrice.items.Num() > 0)
		{
			int itemCount = 0;
			for (FMyItem tradeItems : itemPrice.items)
			{
				itemCount = interactingHeroPack->FindItemCount(tradeItems.id);
				if (itemCount < tradeItems.count)
				{
					CPC->GetHUDManager()->GetMainHUD()->DisplayHelpText(NotEnoughItemsText);
					return;
				}
			}
			interactingHeroPack->RemoveItems(itemPrice.items);
		}

		CPC->GetBasePlayer()->money -= itemPrice.money;
		CPC->GetBasePlayer()->interactedHero->backpack->AddItem(GetBackpack()->GetItem(iSlot));
		CPC->GetHUDManager()->GetInventoryHUD()->LoadItems();
		CPC->GetGameMode()->GetQuestManager()->OnItemPickup(itemToBuy);
	}
	else
		CPC->GetHUDManager()->GetMainHUD()->DisplayHelpText(NotEnoughMoneyText);
}
