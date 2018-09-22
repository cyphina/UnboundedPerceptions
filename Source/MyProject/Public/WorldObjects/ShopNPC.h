// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldObjects/IntimateNPC.h"
#include "Backpack.h"
#include "ShopNPC.generated.h"


/**
 * Depending on your friendship, shopkeepers can sell more things like in TWEWY.  This will probably be handled inside the triggers in IntimateNPC
 */

USTRUCT(Blueprintable, NoExport)
struct FItemPrice 
{
	FItemPrice() : money(0), items(TArray<int>()) {}

	/**How much money this item costs*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int											money;

	/**How many items to trade in for this item*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMyItem>								items;
};

UCLASS()
class MYPROJECT_API AShopNPC : public AIntimateNPC
{
	GENERATED_BODY()
	
	/**
	 *Lists sellable items, and maps them to price
	 */
	UPROPERTY(EditAnywhere, Category =  "Shop Items")
	TMap<int, FItemPrice>						itemPrices;

	void										SetupAppropriateView() override;
	

public:

	static FItemPrice							defaultItemPrice;

	void										BeginPlay() override;

	/**Backpack containing the items that the shopkeeper will sell and how many the shopkeeper can sell before running out of stock
	 * TODO: Setup some kind of list of what the shopkeeper sells based on what part of the story has occured
	 */
	UPROPERTY()
	UBackpack*									itemsToSellBackpack;

	UPROPERTY(EditAnywhere, Category =  "Shop Items")
	TArray<FMyItem>								itemsToSell;

	/**
	 *Accessor to gets an item's price
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FItemPrice&						GetItemPrice(int itemID);
};
