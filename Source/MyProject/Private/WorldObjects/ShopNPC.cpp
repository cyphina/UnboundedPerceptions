// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ShopNPC.h"
#include "UserInput.h"
#include "Items/Backpack.h"
#include "UI/HUDManager.h"
#include "DialogSystem/NPCSocialMenu.h"
#include "RTSIngameWidget.h"
#include "LevelSaveStructs.h"

FItemPrice AShopNPC::defaultItemPrice = FItemPrice();

void AShopNPC::SetupAppropriateView()
{
   controllerRef->GetWidgetProvider()->GetIngameHUD()->GetSocialWindow()->SetShopView();
}

void AShopNPC::BeginPlay()
{
   Super::BeginPlay();
   itemsToSellBackpack = UBackpack::CreateBackpack(this, 20);
   for(FMyItem& item : itemsToSell) {
      itemsToSellBackpack->AddItem(item);
   }
}

FItemPrice& AShopNPC::GetItemPrice(int itemID)
{
   if(itemPrices.Contains(itemID)) { return itemPrices[itemID]; }
   return defaultItemPrice;
}
