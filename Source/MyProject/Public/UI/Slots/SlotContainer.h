// Created 12/20/20 1:52 AM

#pragma once
#include "MyDraggableWidget.h"
#include "SlotContainer.generated.h"

class UActionSlot;
class UActionSlotStyle;

DECLARE_EVENT_OneParam(USlotContainer, FOnSlotSelected, int);

/**
 * @brief Use this widget specifically this to hold action slots if you want to leverage their full powers.
 * You can store them anywhere else like the enemy interface stores skill slots in a tile view, but then you have to a
 * little more work like providing a mechanism to handle slot selection.
 * Note - Each class deriving from this needs to provide its own way of storing the slots created from the actionSlotClass.
 */
UCLASS()
class MYPROJECT_API USlotContainer : public UMyDraggableWidget
{
   GENERATED_BODY()

 public:
   /**
    * @brief Should never have to broadcast this since it is handled already in USlotContainer::NativeOnMouseButonUp()
    */
   FOnSlotSelected& OnSlotSelected() const { return OnSlotSelectedEvent; }

   int GetSelectedSlotIndex() const { return selectedSlotIndex; }

   /** Returns the number of VALID items (not number of slots) */
   UFUNCTION(BlueprintCallable)
   virtual int GetNumValidItems() const PURE_VIRTUAL(USlotContainer::GetNumSlots, return -1;);

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

   /** Index of the slot we clicked set after a mousedown event. */
   int selectedSlotIndex = INDEX_NONE;
};
