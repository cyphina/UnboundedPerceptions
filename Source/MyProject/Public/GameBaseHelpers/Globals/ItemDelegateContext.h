#pragma once
#include "LocalPlayerSubsystem.h"
#include "ItemDelegateContext.generated.h"

class ABaseHero;
struct FMyItem;
struct FBackpackUpdateResult;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemUsed, const ABaseHero*, const FBackpackUpdateResult&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemPickedUp, const ABaseHero*, const FBackpackUpdateResult&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDropped, const ABaseHero*, const FBackpackUpdateResult&);
// Purchasing hero, pack result from adding the new item bought, and pack result from possibly removing existing items
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnItemPurchased, const ABaseHero*, const FBackpackUpdateResult&, const TArray<FBackpackUpdateResult>&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, const ABaseHero*, const FBackpackUpdateResult&);
// Hero that unequipped and the slot in our inventory that's changing. Original backpack. New Backpack. Remove Result. Add Result.
DECLARE_MULTICAST_DELEGATE_FourParams(FOnItemTransferred, const UBackpack&, const UBackpack&, const FBackpackUpdateResult&, const FBackpackUpdateResult&);
DECLARE_MULTICAST_DELEGATE_FourParams(FOnItemsSwapped, const UBackpack&, const UBackpack&, int, int);

UCLASS()
class UItemDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()

public:
   FOnItemUsed& OnItemUsed() { return OnItemUsedEvent; }

   FOnItemPickedUp& OnItemPickedUp() { return OnItemPickedUpEvent; }

   FOnItemDropped& OnItemDropped() { return OnItemDroppedEvent; }

   FOnItemPurchased& OnItemPurchased() { return OnItemPurchasedEvent; }

   FOnEquipmentChanged& OnEquipmentChanged() { return OnEquipmentChangedEvent; }

   FOnItemTransferred& OnItemsTransferred() { return OnItemTransferredEvent; }

   FOnItemsSwapped& OnItemsSwapped() { return OnItemSwappedEvent; }

private:
   FOnItemUsed         OnItemUsedEvent;
   FOnItemPickedUp     OnItemPickedUpEvent;
   FOnItemDropped      OnItemDroppedEvent;
   FOnItemPurchased    OnItemPurchasedEvent;
   FOnEquipmentChanged OnEquipmentChangedEvent;
   FOnItemTransferred  OnItemTransferredEvent;
   FOnItemsSwapped     OnItemSwappedEvent;              
};
