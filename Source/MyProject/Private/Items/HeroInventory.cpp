#include "MyProject.h"
#include "HeroInventory.h"

#include "UserInput.h"
#include "BasePlayer.h"

#include "UI/HUDManager.h"

#include "WorldObjects/BaseHero.h"

<<<<<<< HEAD
#include "ItemDelegateStore.h"
#include "PartyDelegateStore.h"

=======
#include "ItemDelegateContext.h"
#include "PartyDelegateContext.h"
>>>>>>> componentrefactor
#include "AIStuff/AIControllers/HeroAIController.h"
#include "Backpack.h"
#include "InventoryView.h"

FReply UHeroInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(GetSelectedSlotIndex() != INDEX_NONE)
   {
      SetSelectedSlotIndex(inventoryView->GetCorrespondingBackpackIndex(GetSelectedSlotIndex()));
   }
   return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UHeroInventory::NativeOnInitialized()
{
   Super::NativeOnInitialized();
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnEquipmentChanged().AddUObject(this, &UHeroInventory::OnItemEquipped);
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemPickedUp().AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemPurchased().AddUObject(this, &UHeroInventory::OnItemPurchased);
   GetOwningLocalPlayer()->GetSubsystem<UItemDelegateContext>()->OnItemUsed().AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   GetOwningLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnHeroActiveChanged().AddUObject(this, &UHeroInventory::OnHeroActiveChanged);
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

void UHeroInventory::OnItemEquipped(const ABaseHero* heroSwappingEquips, TArray<int> affectedInventoryIndices)
{
   if(CPC->GetHUDManager()->IsWidgetOnScreen(EHUDs::HS_Inventory))
   {
      ReloadSlots(TSet<int>(affectedInventoryIndices));
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

<<<<<<< HEAD
void UHeroInventory::NativeOnInitialized()
{
   ItemChangeEvents::OnEquipmentChangedEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   ItemChangeEvents::OnItemPickedUpEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   ItemChangeEvents::OnItemPurchasedEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   ItemChangeEvents::OnItemUsedEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   GetOwningLocalPlayer()->GetSubsystem<UPartyDelegateStore>()->OnHeroActiveChanged().AddUObject(this, &UHeroInventory::OnHeroActiveChanged);
}

void UHeroInventory::UseItemAtInventorySlot_Implementation(int32 iSlot)
{
   if(!GetBackpack()->IsEmptySlot(iSlot)) {
      OnItemSelected().Broadcast(hIndex, iSlot);
   }
}

void UHeroInventory::OnItemChangeEvent(const ABaseHero* heroUsingItem, const FMyItem& item)
{
   if(GetVisibility() == ESlateVisibility::Visible) LoadItems();
}

void UHeroInventory::OnHeroActiveChanged(ABaseHero* heroThatChangedActivefState, bool newActiveState)
{
   // TODO: Refresh inventory and move from the selected page. Also change hIndex... if we plan to keep it around.
   if(!newActiveState) {
      LoadItems();
=======
void UHeroInventory::OnHeroActiveChanged(ABaseHero* heroThatChangedActiveState, bool newActiveState)
{
   // TODO: Refresh inventory and move from the selected page. Also change hIndex... if we plan to keep it around.
   if(!newActiveState)
   {
      GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
      {
         LoadItems();
      });
>>>>>>> componentrefactor
   }
}
