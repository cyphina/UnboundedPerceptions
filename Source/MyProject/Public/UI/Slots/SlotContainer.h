// Created 12/20/20 1:52 AM

#pragma once
#include "MyDraggableWidget.h"
#include "SlotContainer.generated.h"

class UActionSlot;

DECLARE_EVENT_OneParam(USlotContainer, FOnSlotSelected, int);

UCLASS()
class MYPROJECT_API USlotContainer : public UMyDraggableWidget
{
   GENERATED_BODY()

public:
   FOnSlotSelected& OnSlotSelected() const { return OnSlotSelectedEvent; }

   int GetSelectedSlotIndex() const { return selectedSlotIndex;}

   virtual int GetNumSlots() const PURE_VIRTUAL(USlotContainer::GetNumSlots, return -1;)

   void SetSelectedSlotIndex(int slotIndex)
   {
      check(slotIndex >= 0 && slotIndex < GetNumSlots())
      selectedSlotIndex = slotIndex;
   }

protected:
   /** Handles bubbling slot logic */
   FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   
   TSubclassOf<UActionSlot> actionSlotClass;

private:
   mutable FOnSlotSelected OnSlotSelectedEvent;

   /** Index of the slot we clicked */
   int selectedSlotIndex = INDEX_NONE;
};
