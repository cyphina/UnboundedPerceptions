// Created 1/17/21 01:13 AM

#pragma once
#include "LocalPlayerSubsystem.h"
#include "StatEnums.h"

#include "UIDelegateContext.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAttributePointAllocated, EAttributes, bool);
DECLARE_EVENT_OneParam(ARTSPawn, FOnUnitSlotSelected, AUnit*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillSlotDropped, int, dragSlotIndex, int, dropSlotIndex);

UCLASS()
class MYPROJECT_API UUIDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()
public:
   FOnAttributePointAllocated OnAttributePointAllocated() { return OnAttributePointAllocatedEvent; };

   FOnUnitSlotSelected& OnUnitSlotSelected() const { return OnUnitSlotSelectedEvent; }

   UPROPERTY(BlueprintAssignable, BlueprintCallable)
   FOnSkillSlotDropped OnSkillSlotDroppedEvent;

private:
   mutable FOnUnitSlotSelected OnUnitSlotSelectedEvent;

   FOnAttributePointAllocated OnAttributePointAllocatedEvent;
};
