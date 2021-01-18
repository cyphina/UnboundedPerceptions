#include "MyProject.h"
#include "HeroInventory.h"

#include "UserInput.h"
#include "BasePlayer.h"

#include "UI/HUDManager.h"

#include "WorldObjects/BaseHero.h"

#include "ItemDelegateStore.h"
#include "PartyDelegateContext.h"

#include "AIStuff/AIControllers/HeroAIController.h"

#include "ItemManager.h"

#include "Backpack.h"

void UHeroInventory::NativeOnInitialized()
{
   ItemChangeEvents::OnEquipmentChangedEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   ItemChangeEvents::OnItemPickedUpEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   ItemChangeEvents::OnItemPurchasedEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   ItemChangeEvents::OnItemUsedEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   GetOwningLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnHeroActiveChanged().AddUObject(this, &UHeroInventory::OnHeroActiveChanged);
}

void UHeroInventory::UseItemAtInventorySlot_Implementation(int32 iSlot)
{
   if(!GetBackpack()->IsEmptySlot(iSlot))
   {
      OnItemSelected().Broadcast(hIndex, iSlot);
   }
}

void UHeroInventory::OnItemChangeEvent(const ABaseHero* heroUsingItem, const FMyItem& item)
{
   if(GetVisibility() == ESlateVisibility::Visible)
   {
      LoadItems();
   }
}

void UHeroInventory::OnHeroActiveChanged(ABaseHero* heroThatChangedActivefState, bool newActiveState)
{
   // TODO: Refresh inventory and move from the selected page. Also change hIndex... if we plan to keep it around.
   if(!newActiveState)
   {
      LoadItems();
   }
}
