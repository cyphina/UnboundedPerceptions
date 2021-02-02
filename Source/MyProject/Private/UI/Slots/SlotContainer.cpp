// Created 1/26/21 10:26 AM

#include "MyProject.h"
#include "SlotContainer.h"

void USlotContainer::NativePreConstruct()
{
   Super::NativePreConstruct();

}

FReply USlotContainer::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   if(selectedSlotIndex != INDEX_NONE)
   {
      OnSlotSelected().Broadcast(selectedSlotIndex);
      selectedSlotIndex = INDEX_NONE;
   }
   else
   {
      Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
   }
   return FReply::Handled();
}
