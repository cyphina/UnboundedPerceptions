// Created 1/26/21 10:26 AM

#include "MyProject.h"
#include "SlotContainer.h"

FReply USlotContainer::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
   Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
   OnSlotSelected().Broadcast(selectedSlotIndex);
   return FReply::Handled();
}
