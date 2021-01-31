#include "MyProject.h"
#include "StorageInventory.h"

#include "ActionSlot.h"
#include "Backpack.h"

bool UStorageInventory::OnWidgetAddToViewport_Implementation()
{
   if(GetBackpack())
   {
      const int numWidgetsToDisable = GetInventorySlots().Num() - GetBackpack()->GetItemMax();
      if(numWidgetsToDisable > 0)
      {
         for(int i = 0; i < numWidgetsToDisable; ++i)
         {
            const int slotToDisableIndex = GetBackpack()->GetItemMax() + i;
            GetInventorySlots()[slotToDisableIndex]->SetIsEnabled(false);
         }
      }
      LoadItems();
      return true;
   }
   return false;
}

void UStorageInventory::OnWidgetRemovedFromViewport_Implementation()
{
   OnStorageInventoryClosed().Broadcast();
}
