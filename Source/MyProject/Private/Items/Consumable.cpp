// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Consumable.h"


UConsumable::UConsumable()
{
	itemInfo.itemType = FGameplayTag::RequestGameplayTag("Item.Consumeable");
}

UConsumable::~UConsumable()
{
}
