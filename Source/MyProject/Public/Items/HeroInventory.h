// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory.h"
#include "HeroInventory.generated.h"

class UHeroInventory;

DECLARE_EVENT_TwoParams(UHeroInventory, FOnItemSelected, int, int);
/**
 * Base class for inventory that is each hero has
 */
UCLASS()
class MYPROJECT_API UHeroInventory : public UInventory
{
   GENERATED_BODY()

 public:
   /**Gets the index corresponding to the hero whose inventory we are looking through*/
   int GetHeroIndex() const { return hIndex; }

   FOnItemSelected& OnItemSelected() { return OnItemSelectedEvent; }

 protected:
   /**Changes color of item if its currently going to be used*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory Functions")
   void SetItemSelected();

   void NativeOnInitialized() override;

   /**Runs when an itemSlot in the inventoryView is clicked on.  Depending on the inventory type, different things may occur.*/
   void UseItemAtInventorySlot_Implementation(int32 iSlot) override;

   /** Party index of the hero which corresponds to the inventory */
   UPROPERTY(BlueprintReadWrite)
   int hIndex;

 private:
   void OnItemChangeEvent(const ABaseHero* heroUsingItem, const FMyItem& item);
   void OnHeroActiveChanged(ABaseHero* heroThatChangedActivefState, bool newActiveState);

   FOnItemSelected OnItemSelectedEvent;
};
