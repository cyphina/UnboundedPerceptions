// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory.h"
#include "HeroInventory.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UHeroInventory : public UInventory
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	int					hIndex;

	/**Runs when an itemSlot in the inventoryView is clicked on.  Depending on the inventory type, different things may occur.*/
	void				UseItemAtInventorySlot_Implementation(int32 iSlot) override;

	/**Changes color of item if its currently going to be used*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory Functions")
	void				SetItemSelected();

};
