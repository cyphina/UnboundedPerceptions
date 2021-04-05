#include "MyProject.h"
#include "ItemSlot.h"

#include "EquipmentMenu.h"
#include "HeroInventory.h"
#include "HUDManager.h"
#include "Items/Inventory.h"
#include "ItemFunctionLibrary.h"
#include "RTSItemDrag.h"
#include "StorageInventory.h"
#include "ToolTipWidget.h"
#include "UIDelegateContext.h"
#include "Items/ItemManager.h"
#include "Items/Backpack.h"

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
   if(inventoryRef->GetBackpackItemAtSlot(slotIndex))
   {
      UDraggedActionWidget* dragVisual = CreateDragIndicator();
      dragVisual->SetDraggedImage(UItemFunctionLibrary::GetItemInfo(inventoryRef->GetBackpackItemAtSlot(slotIndex).id).image);

      URTSItemDrag* dragOp        = NewObject<URTSItemDrag>(this);
      dragOp->Pivot               = EDragPivot::CenterCenter;
      dragOp->DefaultDragVisual   = dragVisual;
      dragOp->slotToBackpackIndex = inventoryRef->GetCorrespondingBackpackIndex(slotIndex);
      dragOp->backpack            = inventoryRef->GetBackpack();
      dragOp->dragWidget          = inventoryRef;
      OutOperation                = MoveTemp(dragOp);
   }
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
   if(const URTSItemDrag* itemDrag = Cast<URTSItemDrag>(InOperation))
   {
      if(UHeroInventory* heroInventory = Cast<UHeroInventory>(itemDrag->dragWidget))
      {
         GetOwningLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnItemSlotDroppedInventoryEvent.Broadcast(itemDrag->slotToBackpackIndex, slotIndex,
                                                                                                               itemDrag->backpack, inventoryRef->GetBackpack());
      }
      else if(UStorageInventory* storageInventory = Cast<UStorageInventory>(itemDrag->dragWidget))
      {
         GetOwningLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnItemSlotDroppedStorageEvent.Broadcast(itemDrag->slotToBackpackIndex, slotIndex, itemDrag->backpack,
                                                                                                             inventoryRef->GetBackpack());
      }
      else
      {
         UE_LOG(LogTemp, Error, TEXT("Item drag's 'dragged widget' reference does not have a valid base class!"));
      }
      return true;
   }
   return false;
}

void UItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(inventoryRef->GetBackpackItemAtSlot(slotIndex))
   {
      Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
   }
}

void UItemSlot::ShowDesc(UToolTipWidget* tooltip)
{
   const FMyItem item = inventoryRef->GetBackpackItemAtSlot(slotIndex);
   if(item)
   {
      const auto itemInfo = UItemManager::Get().GetItemInfo(item.id);
      if(itemInfo->itemType.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Equippable", false)))
      {
         const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
         const FText bonusDesc  = UItemFunctionLibrary::GetBonusDescription(item.id);
         tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, bonusDesc, FText::GetEmpty());
      }
      // Else we don't have a piece of equipment
      else
      {
         const FText rarityName = UItemFunctionLibrary::GetRarityText(itemInfo->rarity);
         tooltip->SetupTTBoxText(itemInfo->name, rarityName, itemInfo->description, FText::GetEmpty(), FText::GetEmpty());
      }
   }
}
