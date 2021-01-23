#pragma once
#include "LocalPlayerSubsystem.h"
#include "ItemDelegateContext.generated.h"

class ABaseHero;
struct FMyItem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemUsed, const ABaseHero*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemPickedUp, const ABaseHero*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDropped, const ABaseHero*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemPurchased, const ABaseHero*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, const ABaseHero*, const FMyItem&); // Hero that unequipped and the slot in our inventory that's changing
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBackpackUpdated, UObject*);                           // Hero that has backpack updated

UCLASS()
class UItemDelegateContext : public ULocalPlayerSubsystem
{
   GENERATED_BODY()

public:
   FOnItemUsed&         OnItemUsed() { return OnItemUsedEvent; }
   FOnItemPickedUp&     OnItemPickedUp() { return OnItemPickedUpEvent; }
   FOnItemDropped&      OnItemDropped() { return OnItemDroppedEvent; }
   FOnItemPurchased&    OnItemPurchased() { return OnItemPurchasedEvent; }
   FOnEquipmentChanged& OnEquipmentChanged() { return OnEquipmentChangedEvent; }
   FOnBackpackUpdated&  OnBackpackUpdated() { return OnBackpackUpdatedEvent; }

private:
   FOnItemUsed         OnItemUsedEvent;
   FOnItemPickedUp     OnItemPickedUpEvent;
   FOnItemDropped      OnItemDroppedEvent;
   FOnItemPurchased    OnItemPurchasedEvent;
   FOnEquipmentChanged OnEquipmentChangedEvent;
   FOnBackpackUpdated  OnBackpackUpdatedEvent;
};
