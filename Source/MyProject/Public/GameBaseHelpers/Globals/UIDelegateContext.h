// Created 1/17/21 01:13 AM

#pragma once
#include "LocalPlayerSubsystem.h"
#include "StatEnums.h"
#include "UIDelegateContext.generated.h"

class ABaseHero;
class UBackpack;

// Hero whose stat got upgraded, Attribute that got upgraded, and was T/F = upgraded/downgraded
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributePointAllocated, ABaseHero*, EAttributes, bool);
DECLARE_EVENT_OneParam(ARTSPawn, FOnUnitSlotSelected, AUnit*);      // Pass in unit corresponding to unit lost
DECLARE_EVENT_OneParam(UStoreInventory, FOnStoreSlotSelected, int); // Pass in slot index
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillSlotDropped, int, dragSlotIndex, int, dropSlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnItemSlotDropped, int, dragSlotIndex, int, dropSlotIndex, UBackpack*, dragPack, UBackpack*, dropPack);


UCLASS()
class MYPROJECT_API UUIDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()
public:
   FOnAttributePointAllocated& OnAttributePointAllocated() { return OnAttributePointAllocatedEvent; }

   FOnUnitSlotSelected& OnUnitSlotSelected() const { return OnUnitSlotSelectedEvent; }

   FOnStoreSlotSelected& OnStoreSlotSelected() { return OnStoreSlotSelectedEvent; };

   UPROPERTY(BlueprintAssignable, BlueprintCallable)
   FOnSkillSlotDropped OnSkillSlotDroppedEvent;

   UPROPERTY(BlueprintAssignable, BlueprintCallable)
   FOnItemSlotDropped OnItemSlotDroppedEvent;

private:
   mutable FOnUnitSlotSelected OnUnitSlotSelectedEvent;

   FOnAttributePointAllocated OnAttributePointAllocatedEvent;

   FOnStoreSlotSelected OnStoreSlotSelectedEvent;
};
