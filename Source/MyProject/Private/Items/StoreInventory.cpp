// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "StoreInventory.h"
#include "UserInput.h"
#include "UI/HUDManager.h"

#include "RTSGameMode.h"
#include "QuestManager.h"

#include "UI/HUDManager.h"
#include "UI/UserWidgets/MainWidget.h"
#include "UI/UserWidgets/MainWidget.h"
#include "Items/HeroInventory.h"

#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/ShopNPC.h"
#include "ItemManager.h"

FText UStoreInventory::NotEnoughItemsText = NSLOCTEXT("HelpMessages", "Items", "Missing required items for trade");
FText UStoreInventory::NotEnoughMoneyText = NSLOCTEXT("HelpMessages", "Items", "You don't have enough squeezies...");
FText UStoreInventory::confirmTitleText = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirmTitle", "Purchase");
FText UStoreInventory::ensurePurchaseText = NSLOCTEXT("HelpMessages", "ItemPurchaseConfirm", "Are you sure you want to buy this?");

bool UStoreInventory::OnItemPurchased()
{
	interactingHeroPack->RemoveItems(itemPrice->items);
	CPC->GetBasePlayer()->money -= itemPrice->money;
	FMyItem newItemPurchased{ itemToBuy, 1 };
	CPC->GetBasePlayer()->interactedHero->backpack->AddItem(newItemPurchased);
	CPC->GetHUDManager()->GetInventoryHUD()->LoadItems();
	CPC->GetGameMode()->GetQuestManager()->OnItemPickup(newItemPurchased);
	return true;
}

bool UStoreInventory::OnItemsPurchased(FString howManyItems)
{
	if (howManyItems != "")
	{
		int num = FCString::Atoi(*howManyItems);
		if (EnoughFunds(num))
		{
			TArray<FMyItem> itemPriceItems = itemPrice->items;
			for (FMyItem& item : itemPriceItems)
			{
				item.count *= num;
			}

			interactingHeroPack->RemoveItems(itemPriceItems);
			CPC->GetBasePlayer()->money -= itemPrice->money * num;
			FMyItem newItemPurchased{ itemToBuy, num };
			CPC->GetBasePlayer()->interactedHero->backpack->AddItem(newItemPurchased);
			CPC->GetHUDManager()->GetInventoryHUD()->LoadItems();
			CPC->GetGameMode()->GetQuestManager()->OnItemPickup(newItemPurchased);
			return true;
		}
	}
	return false;
}

bool UStoreInventory::EnoughFunds(int numPurchasing)
{
	if (itemPrice->money * numPurchasing <= CPC->GetBasePlayer()->money)
	{
		if (itemPrice->items.Num() > 0)
		{
			int itemCount = 0;
			for (FMyItem& tradeItems : itemPrice->items)
			{
				itemCount = interactingHeroPack->FindItemCount(tradeItems.id);
				if (itemCount < tradeItems.count * numPurchasing)
				{
					CPC->GetHUDManager()->GetMainHUD()->DisplayHelpText(NotEnoughItemsText);
					return false;
				}
			}
		}
		return true;
	}
	CPC->GetHUDManager()->GetMainHUD()->DisplayHelpText(NotEnoughMoneyText);
	return false;
}

void UStoreInventory::UseItemAtInventorySlot_Implementation(int32 iSlot)
{
	itemSlot = iSlot;
	itemToBuy = GetBackpack()->GetItem(itemSlot).id;
	itemPrice = &shopkeeper->GetItemPrice(itemToBuy);

	//Check to make sure the hero doesn't die while talking to the shopkeeper... which could be to poison or to an enemy if enemies spawn in the same room
	if (IsValid(CPC->GetBasePlayer()->interactedHero))
		interactingHeroPack = CPC->GetBasePlayer()->interactedHero->backpack;

	if (!UItemManager::Get().GetItemInfo(itemToBuy)->isStackable)
	{
		if (EnoughFunds(1))
			CPC->GetHUDManager()->AddHUDConfirm("OnItemPurchased", this, confirmTitleText, ensurePurchaseText);
	}
	else
		CPC->GetHUDManager()->AddHUDInput("OnItemsPurchased", this, confirmTitleText, ensurePurchaseText);
}
