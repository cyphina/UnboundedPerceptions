#pragma once

#include "CoreMinimal.h"
#include "Items/Inventory.h"
#include "HeroInventory.generated.h"

<<<<<<< HEAD
class UHeroInventory;

DECLARE_EVENT_TwoParams(UHeroInventory, FOnItemSelected, int, int);
=======
struct FBackpackUpdateResult;
class UHeroInventory;

>>>>>>> componentrefactor
/**
 * Base class for inventory that is each hero has. The inventory menu has 4 tabs at the top corresponding to the backpacks of the different heroes in your party.
 */
UCLASS()
class MYPROJECT_API UHeroInventory : public UInventory
{
   GENERATED_BODY()

<<<<<<< HEAD
 public:
   /**Gets the index corresponding to the hero whose inventory we are looking through*/
   int GetHeroIndex() const { return hIndex; }

   FOnItemSelected& OnItemSelected() { return OnItemSelectedEvent; }

 protected:
=======
public:
   /**Gets the index corresponding to the hero whose inventory we are looking through*/
   int GetHeroIndex() const { return hIndex; }

protected:
   FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   
>>>>>>> componentrefactor
   /**Changes color of item if its currently going to be used*/
   UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Inventory Functions")
   void SetItemSelected();

   void NativeOnInitialized() override;

<<<<<<< HEAD
   /**Runs when an itemSlot in the inventoryView is clicked on.  Depending on the inventory type, different things may occur.*/
   void UseItemAtInventorySlot_Implementation(int32 iSlot) override;

=======
>>>>>>> componentrefactor
   /** Party index of the hero which corresponds to the inventory */
   UPROPERTY(BlueprintReadWrite)
   int hIndex;

<<<<<<< HEAD
 private:
   void OnItemChangeEvent(const ABaseHero* heroUsingItem, const FMyItem& item);
   void OnHeroActiveChanged(ABaseHero* heroThatChangedActivefState, bool newActiveState);

   FOnItemSelected OnItemSelectedEvent;
=======
private:
   void OnItemChangeEvent(const ABaseHero* heroUsingItem, const FBackpackUpdateResult& packUpdateResult);
   void OnItemEquipped(const ABaseHero* heroSwappingEquips, TArray<int> affectedInventoryIndices);
   void OnItemPurchased(const ABaseHero* heroRef, const FBackpackUpdateResult& addItemResult, const TArray<FBackpackUpdateResult>& removeItemsResults);
   void OnHeroActiveChanged(ABaseHero* heroThatChangedActiveState, bool newActiveState);
>>>>>>> componentrefactor
};
