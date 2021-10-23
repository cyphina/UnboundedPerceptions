#pragma once

#include "CoreMinimal.h"

#include "BackpackStructs.h"
#include "Items/Inventory.h"
#include "StoreInventory.generated.h"

class AShopNPC;
class UBackpack;
struct FMyItem;
struct FItemPrice;

/**
* Inventory for shops to buy things in game for your units!
*/
UCLASS()
class MYPROJECT_API UStoreInventory : public UInventory
{
   GENERATED_BODY()

public:
   UFUNCTION()
   AShopNPC* GetShopkeeper() const { return shopkeeper; }
   
protected:
   void NativeOnInitialized() override;
   bool OnWidgetAddToViewport_Implementation() override;

private:
   void OnItemPurchased(const ABaseHero* heroRef, const FBackpackUpdateResult& addItemResult, const TArray<FBackpackUpdateResult>& removeItemsResults);

   UPROPERTY()
   class AHUDManager* hudManagerRef;

   AShopNPC* shopkeeper;
};
