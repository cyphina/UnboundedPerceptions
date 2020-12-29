// Created 12/20/20 1:52 AM

#pragma once
#include "UserWidget.h"
#include "SlotContainer.generated.h"

class UActionSlot;

DECLARE_EVENT_OneParam(USlotContainer, FOnSlotSelected, int);

UCLASS()
class MYPROJECT_API USlotContainer : public UUserWidget
{
   GENERATED_BODY()

 public:
   FOnSlotSelected& OnSlotSelected() const { return OnSlotSelectedEvent; }

 private:
   mutable FOnSlotSelected OnSlotSelectedEvent;
   TArray<UActionSlot*>    actionSlots;
};
