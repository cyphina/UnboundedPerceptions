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
   
   void SetSelectedSlotIndex(int slotIndex)
   {
      check(slotIndex >= 0 && slotIndex < actionSlots.Num())
      selectedSlotIndex = slotIndex;
   }
   
protected:
   /** Handles bubbling slot logic */
   FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
   
   TSubclassOf<UActionSlot> actionSlotClass;

   UPROPERTY(BlueprintReadWrite)
   TArray<UActionSlot*>    actionSlots;

private:
   mutable FOnSlotSelected OnSlotSelectedEvent;

   /** Index of the slot we clicked */
   int selectedSlotIndex = 0;
};
