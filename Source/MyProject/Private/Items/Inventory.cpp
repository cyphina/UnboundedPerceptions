#include "MyProject.h"
#include "Inventory.h"
#include "Backpack.h"
#include "BaseHero.h"
#include "InventoryView.h"

void UInventory::UseItem(int32 iSlot)
{
   if(!GetBackpack()->IsEmptySlot(iSlot))
   {
      UseItemAtInventorySlot(inventoryView->GetCorrespondingBackpackIndex(iSlot));
      LoadItems();
   }
}

FMyItem UInventory::GetBackpackItemAtSlot(int slotIndex) const
{
   return GetBackpack()->GetItem(inventoryView->GetCorrespondingBackpackIndex(slotIndex));
}

void UInventory::NativeConstruct()
{
   Super::NativeConstruct();
}

bool UInventory::OnWidgetAddToViewport_Implementation()
{
   // We still need to fix this to go before we add it to the hud
   Super::OnWidgetAddToViewport_Implementation();

   if(!backpack) return false;
   LoadItems();
   return true;
}
