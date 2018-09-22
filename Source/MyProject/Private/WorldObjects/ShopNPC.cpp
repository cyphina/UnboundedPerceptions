// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ShopNPC.h"
#include "UserInput.h"
#include "Items/Backpack.h"
#include "UI/HUDManager.h"
#include "DialogSystem/DialogUI.h"
#include "LevelSaveStructs.h"

FItemPrice AShopNPC::defaultItemPrice = FItemPrice();

void AShopNPC::SetupAppropriateView()
{
	controllerRef->GetHUDManager()->GetSocialWindow()->SetShopView();
}

void AShopNPC::BeginPlay()
{
	Super::BeginPlay();
	itemsToSellBackpack = NewObject<UBackpack>(this);
	for(FMyItem& item : itemsToSell)
	{
		itemsToSellBackpack->AddItem(item);
	}
}

FItemPrice& AShopNPC::GetItemPrice(int itemID)
{
	if(itemPrices.Contains(itemID))
	{
		return itemPrices[itemID];
	}
	return defaultItemPrice;
}
