#pragma once

class ABaseHero;
struct FMyItem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemUsed, const ABaseHero*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemPickedUp, const ABaseHero*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDropped, const ABaseHero*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemPurchased, const ABaseHero*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, const ABaseHero*, const FMyItem&); // Hero that unequipped and the slot in our inventory that's changing

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemAddedToInventory, UObject*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemRemovedFromInventory, UObject*, const FMyItem&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemOverflow, UObject*, const FMyItem&);

namespace ItemChangeEvents
{
   inline FOnItemUsed         OnItemUsedEvent;
   inline FOnItemPickedUp     OnItemPickedUpEvent;
   inline FOnItemDropped      OnItemDroppedEvent;
   inline FOnItemPurchased    OnItemPurchasedEvent;
   inline FOnEquipmentChanged OnEquipmentChangedEvent;

   inline FOnItemOverflow             OnItemOverflowEvent;
   inline FOnItemAddedToInventory     OnItemAddedToInventoryEvent;
   inline FOnItemRemovedFromInventory OnItemRemovedFromInventoryEvent;
}
