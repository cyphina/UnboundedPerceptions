// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "HeroInventory.h"

#include "UserInput.h"
#include "BasePlayer.h"

#include "UI/HUDManager.h"

#include "WorldObjects/BaseHero.h"

#include "ItemDelegateStore.h"

#include "AIStuff/AIControllers/HeroAIController.h"

#include "ItemManager.h"

#include "Backpack.h"

void UHeroInventory::NativeOnInitialized()
{
   ItemChangeEvents::OnEquipmentChangedEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   ItemChangeEvents::OnItemPickedUpEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   ItemChangeEvents::OnItemPurchasedEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
   ItemChangeEvents::OnItemUsedEvent.AddUObject(this, &UHeroInventory::OnItemChangeEvent);
}

void UHeroInventory::UseItemAtInventorySlot_Implementation(int32 iSlot)
{
   if(!GetBackpack()->IsEmptySlot(iSlot)) {
      const FMyItem      itemUsed = GetBackpack()->GetItem(iSlot);
      const FGameplayTag itemType = UItemManager::Get().GetItemInfo(itemUsed.id)->itemType;
      ABaseHero*         hero     = CPC->GetBasePlayer()->GetHeroes()[hIndex];

      if(itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Equippable"))) {
         if(hero) { hero->Equip(iSlot); }
      } else if(itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Consumeable"))) {
         hero->GetHeroController()->BeginUseItem(itemUsed.id, iSlot);
      } else {
         UE_LOG(LogTemp, Log, TEXT("ERROR WITH ITEM TYPE"));
      }
   }
}

void UHeroInventory::OnItemChangeEvent(const ABaseHero* heroUsingItem, const FMyItem& item)
{
   if(GetVisibility() == ESlateVisibility::Visible) LoadItems();
}
