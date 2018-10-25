// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "CollectiblesUI.h"

void UCollectiblesUI::AddItemToUI(FText ItemName)
{
   ItemToAdd = ItemName;
   AddItemToUI();
}
