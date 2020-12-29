// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Inventory.h"

void UInventory::NativeConstruct()
{
   Super::NativeConstruct();
}

bool UInventory::OnWidgetAddToViewport_Implementation()
{
   // We still need to fix this to go before we add it to the hud
   Super::OnWidgetAddToViewport_Implementation();

   if(!backpack) return false;
   LoadItems();
   return true;
}
