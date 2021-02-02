// Created 12/20/20 1:52 AM

#pragma once
#include "MyDraggableWidget.h"
#include "SlotContainer.generated.h"

class UActionSlot;
class UActionSlotStyle;

DECLARE_EVENT_OneParam(USlotContainer, FOnSlotSelected, int);

UCLASS()
class MYPROJECT_API USlotContainer : public UMyDraggableWidget
{
   GENERATED_BODY()

public:
   FOnSlotSelected& OnSlotSelected() const { return OnSlotSelectedEvent; }

   int GetSelectedSlotIndex() const { return selectedSlotIndex;}

   /** Returns the number of VALID items (not number of slots) */
   virtual int GetNumValidItems() const PURE_VIRTUAL(USlotContainer::GetNumSlots, return -1;)

   void SetSelectedSlotIndex(int slotIndex)
   {
      if(slotIndex >= INDEX_NONE && slotIndex < GetNumValidItems())
      {
         selectedSlotIndex = slotIndex;
      }
      else
      {
         UE_LOG(LogTemp, Error, TEXT("Tried to select slot index that was out of bounds!"));
      }
   }

protected:
   void NativePreConstruct() override;

   /** Handles tunneling slot logic */
   FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

   TSubclassOf<UActionSlot> actionSlotClass;

private:
   mutable FOnSlotSelected OnSlotSelectedEvent;

   /** Index of the slot we clicked */
   int selectedSlotIndex = INDEX_NONE;
};
