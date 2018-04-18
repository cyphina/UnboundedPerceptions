// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/Item.h"
#include "UI/UserWidgetExtensions/MyDraggableWidget.h"
#include "Inventory.generated.h"

class UBackpack;
class ABaseHero;
class UInventoryView;

/*Inventory C++ base class.  Widgets don't get constructors.  Can be used as UI for chacter inventory and for treasure chest UI and such.
 *All Inventory does is display items in a backpack and lets us use them and maybe reorder them*/

UCLASS(Blueprintable)
class MYPROJECT_API UInventory : public UMyUserWidget
{
	GENERATED_BODY()

public:
	virtual void		Construct_Implementation() override;	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Callback")
	bool				OnWidgetAddToViewport();
	bool				OnWidgetAddToViewport_Implementation() override;

	/**Uses item at a given inventory slot.  Triggers when item is clicked on  If some kind of container's inventory, just loot the items quickly
	 *ONLY "USES" IN HERO INVENTORY, in other inventories use is collect */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Functions")
	void				UseItemAtInventorySlot(ABaseHero* hero, int32 iSlot);
	
	/**Swap location slot in backpack of two items*/
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void				SwapItems(UBackpack* otherBackpack, int32 iSlot, int32 iSlot2);

	/**Swap what actor the inventory is referencing (you can also just swap backpacks)*/
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void				SwapInventoryActors(AActor* actorWInventory);

	/**Used to update the view whenever change occurs within our inventory*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory Functions")
	void				LoadItems();

	/**Changes color of item if its currently going to be used*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory Functions")
	void				SetItemSelected();

	/**Accessors for backpack*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory Functions")
	UBackpack*			GetBackpack() const { return backpack; }

	/**Accessors for backpack*/
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void				SetBackPack(UBackpack* bPack){	backpack = bPack;}

	/**a reference to our main inventory UI widget*/
	UPROPERTY(BlueprintReadWrite) 
	UInventoryView*		inventoryView;

private:
	/**backpack reference for whatever inventory we are displaying*/
	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UIInitialParams", Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	UBackpack*			backpack;

	///information on number of rows and columns
	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UIInitialParams", Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	int					rows;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, category = "UIInitialParams", Meta = (AllowPrivateAccess = true, ExposeOnSpawn = true))
	int					columns;
};
