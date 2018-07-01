// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory.h"
#include "StoreInventory.generated.h"

/**
 * Inventory for shops
 */

class AShopNPC;

UCLASS()
class MYPROJECT_API UStoreInventory : public UInventory
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	AShopNPC* shopkeeper;

	void	UseItemAtInventorySlot_Implementation(int32 iSlot) override;
	
};
