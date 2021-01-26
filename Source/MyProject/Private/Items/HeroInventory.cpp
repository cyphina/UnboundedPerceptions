#include "MyProject.h"
#include "HeroInventory.h"

#include "UserInput.h"
#include "BasePlayer.h"

#include "UI/HUDManager.h"

#include "WorldObjects/BaseHero.h"

#include "ItemDelegateContext.h"
#include "PartyDelegateContext.h"

#include "AIStuff/AIControllers/HeroAIController.h"

#include "ItemManager.h"

#include "Backpack.h"

void UHeroInventory::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnEquipmentChanged().AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemPickedUp().AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemPurchased().AddUObject(this, &UHeroInventory::OnItemPurchased);
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemUsed().AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   GetOwningLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnHeroActiveChanged().AddUObject(this, &UHeroInventory::OnHeroActiveChanged);
}

void UHeroInventory::UseItemAtInventorySlot_Implementation(int32 iSlot)
{
   if(!GetBackpack()->IsEmptySlot(iSlot))
   {
      OnInventoryItemSelected().Broadcast(hIndex, iSlot);
   }
}

void UHeroInventory::OnItemChangeEvent(const ABaseHero* heroUsingItem, const FBackpackUpdateResult& packUpdateResult)
{
   if(CPC->GetHUDManager()->IsWidgetOnScreen(EHUDs::HS_Inventory))
   {
      GetWorld()->GetTimerManager().SetTimerForNextTick([this, packUpdateResult]()
      {
         ReloadSlots(TSet<int>(packUpdateResult.updatedBackpackIndices));
      });
   }
}

void UHeroInventory::OnItemPurchased(const ABaseHero* heroRef, const FBackpackUpdateResult& addItemResult, const TArray<FBackpackUpdateResult>& removeItemsResults)
{
   TSet<int> updatedSlotIndices;
   updatedSlotIndices.Append(addItemResult.updatedBackpackIndices);
   for(const FBackpackUpdateResult& removeItemsResult : removeItemsResults)
   {
      updatedSlotIndices.Append(removeItemsResult.updatedBackpackIndices);
   }

   ReloadSlots(updatedSlotIndices);
}


void UHeroInventory::OnHeroActiveChanged(ABaseHero* heroThatChangedActivefState, bool newActiveState)
{
   // TODO: Refresh inventory and move from the selected page. Also change hIndex... if we plan to keep it around.
   if(!newActiveState)
   {
      GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
      {
         LoadItems();
      });
   }
}
