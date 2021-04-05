#include "MyProject.h"
#include "StoreInventory.h"
#include "UserInput.h"
#include "UI/HUDManager.h"
#include "Items/HeroInventory.h"

#include "BasePlayer.h"
#include "WorldObjects/BaseHero.h"
#include "WorldObjects/ShopNPC.h"
#include "ItemManager.h"

#include "ItemDelegateContext.h"
#include "UIDelegateContext.h"

void UStoreInventory::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemPurchased().AddUObject(this, &UStoreInventory::OnItemPurchased);
}

bool UStoreInventory::OnWidgetAddToViewport_Implementation()
{
   if(ABaseHero* blockerHero = CPC->GetBasePlayer()->heroInBlockingInteraction)
   {
      if(blockerHero->GetCurrentInteractable())
      {
         shopkeeper = Cast<AShopNPC>(CPC->GetBasePlayer()->heroInBlockingInteraction->GetCurrentInteractable());
         if(shopkeeper)
         {
            SetBackPack(shopkeeper->GetItemsToSell());
            return Super::OnWidgetAddToViewport_Implementation();
         }
      }
   }
   return false;
}

void UStoreInventory::OnItemPurchased(const ABaseHero* heroRef, const FBackpackUpdateResult& addItemResult, const TArray<FBackpackUpdateResult>& removeItemsResults)
{
   TSet<int> updatedSlotIndices;
   updatedSlotIndices.Append(addItemResult.updatedBackpackIndices);
   for(const FBackpackUpdateResult& removeItemsResult : removeItemsResults)
   {
      updatedSlotIndices.Append(removeItemsResult.updatedBackpackIndices);
   }

   ReloadSlots(updatedSlotIndices);
}
