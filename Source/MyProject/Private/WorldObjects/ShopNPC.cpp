// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "ShopNPC.h"


FItemPrice AShopNPC::GetItemPrice(int itemID)
{
	if(itemPrices.Contains(itemID))
	{
		return itemPrices[itemID];
	}
	return FItemPrice();
}
