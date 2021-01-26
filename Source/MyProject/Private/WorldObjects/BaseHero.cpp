#include "MyProject.h"

#include "BasePlayer.h"
#include "RTSGameMode.h"
#include "UserInput.h"
#include "RTSPawn.h"

#include "Items/EquipmentMenu.h"
#include "UI/HUDManager.h"

#include "BaseHero.h"
#include "AIStuff/AIControllers/HeroAIController.h"

#include "Trigger.h"

#include "ItemManager.h"
#include "Items/Consumable.h"
#include "Items/Equip.h"
#include "Items/EquipmentContainer.h"

#include "AbilitySystemComponent.h"
#include "CombatParameters.h"
#include "GameplayDelegateContext.h"
#include "HeroInventory.h"
#include "ItemDelegateContext.h"
#include "PartyDelegateContext.h"
#include "RTSIngameWidget.h"
#include "RTSSidebarWidget.h"
#include "TargetComponent.h"
#include "UpStatComponent.h"
#include "Components/SpellCastComponent.h"
#include "SpellSystem/MySpell.h"
#include "SpellSystem/Spellbook.h"
#include "WorldObjects/NPC.h"
#include "ItemDelegateContext.h"
#include "ShopNPC.h"
#include "SpellDelegateStore.h"
#include "StorageContainer.h"
#include "UIDelegateContext.h"

ABaseHero::ABaseHero(const FObjectInitializer& oI) :
   AAlly(oI)
{
}

void ABaseHero::BeginPlay()
{
   Super::BeginPlay();

   player = Cast<ABasePlayer>(GetWorld()->GetGameInstance()->GetFirstLocalPlayerController()->PlayerState);
   player->allHeroes.Add(this);

   currentInteractable = nullptr;

   backpack = UBackpack::CreateBackpack(this, 40);

   equipment = NewObject<UEquipmentContainer>(this, "Equipment");
   equipment->OnEquipmentContainerChanged().BindUObject(this, &ABaseHero::OnEquipped);

   GetHeroController()->GetSpellCastComponent()->OnSpellCasted().AddUObject(this, &ABaseHero::OnSpellCasted);

   LoadSavedTriggers();
   GiveItemAbilities();

   spellbook = USpellBook::CreateSpellBook(this);
   SpellHUDEvents::OnSpellLearnedEvent.AddUObject(this, &ABaseHero::OnSpellLearned);
   SpellHUDEvents::OnSpellUpgradedEvent.AddUObject(this, &ABaseHero::OnSpellUpgraded);

   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().AddUObject(this,
      &ABaseHero::OnAttributePointAllocated);
   OnPickupItem().BindUObject(this, &ABaseHero::OnItemPickup);
}

void ABaseHero::Tick(float deltaSeconds)
{
   Super::Tick(deltaSeconds);
}

void ABaseHero::EndPlay(EEndPlayReason::Type epr)
{
   Super::EndPlay(epr);
}

void ABaseHero::PossessedBy(AController* newController)
{
   Super::PossessedBy(newController);
   heroController = Cast<AHeroAIController>(GetController());
   OnUnitDie().AddUObject(this, &ABaseHero::CheckGameOverOnDeath);
}

void ABaseHero::UnPossessed()
{
   Super::UnPossessed();
   OnUnitDie().RemoveAll(this);
}

void ABaseHero::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnHeroActiveChanged().Broadcast(this, bEnabled);
   if(bEnabled)
   {
      controllerRef->GetHUDManager()->GetIngameHUD()->GetInventoryHUD()->OnInventoryItemSelected().AddUObject(this, &ABaseHero::OnInventoryItemSelected);
   } else
   {
      controllerRef->GetHUDManager()->GetIngameHUD()->GetInventoryHUD()->OnInventoryItemSelected().RemoveAll(this);
   }
}

void ABaseHero::CheckGameOverOnDeath() const
{
   bool isAllDead = true;
   for(const ABaseHero* hero : controllerRef->GetBasePlayer()->GetHeroes())
   {
      if(!hero->GetCombatInfo()->isDead)
      {
         isAllDead = false;
         break;
      }
   }
   if(isAllDead)
      controllerRef->GetGameMode()->GameOver();
}

int ABaseHero::GetCurrentExp() const
{
   return currentExp;
}

int ABaseHero::GetExpToLevel() const
{
   return expForLevel;
}

void ABaseHero::SetCurrentItem(int newCurrentItemID, int newCurrentItemSlotIndex)
{
   currentItemId        = newCurrentItemID;
   currentItemSlotIndex = newCurrentItemSlotIndex;
}

void ABaseHero::SetCurrentExp(int amount)
{
   currentExp += amount;
   while(currentExp >= expForLevel)
   {
      currentExp = currentExp - expForLevel;
      expForLevel *= NEXT_EXP_MULTIPLIER;
      LevelUp();
   }
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UGameplayDelegateContext>()->OnExpGained().Broadcast(amount);
}

const UEquipmentContainer* ABaseHero::GetEquipment() const
{
   return equipment;
}

void ABaseHero::LevelUp_Implementation()
{
   attPoints += 5;
   skillPoints += 1;
   statComponent->SetUnitLevel(statComponent->GetUnitLevel() + 1);
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnHeroLevelUp().Broadcast(this);
}

void ABaseHero::SetCurrentInteractable(AActor* newInteractable)
{
   currentInteractable = newInteractable;
   GetTargetComponent()->SetTarget(newInteractable);
}

void ABaseHero::OnAttributePointAllocated(ABaseHero* heroWithAttChange, EAttributes att, bool isIncrementing)
{
   if(heroWithAttChange == this)
   {
      if(isIncrementing)
      {
         GetStatComponent()->ModifyStats<true>(statComponent->GetAttributeBaseValue(att) + 1, att);
         --attPoints;
         GetStatComponent()->UpdateStats(UMyAttributeSet::IndexAtts(static_cast<int>(att)));
      } else
      {
         GetStatComponent()->ModifyStats<true>(statComponent->GetAttributeBaseValue(att) - 1, att);
         ++attPoints;
         GetStatComponent()->UpdateStats(UMyAttributeSet::IndexAtts(static_cast<int>(att)));
      }
   }
}

void ABaseHero::Equip(const int backpackIndex)
{
   const int prevEquipInSlot = equipment->Equip(backpack->GetItem(backpackIndex).id);
   backpack->RemoveItemAtSlot(backpackIndex);

   if(prevEquipInSlot)
   {
      const FBackpackUpdateResult addItemToPackResult = backpack->AddItem(FMyItem(prevEquipInSlot));
      if(!ensure(addItemToPackResult.bSuccessfulOperation))
      {
         UE_LOG(LogTemp, Error, TEXT("Impossible case reached. No space to swap equipment even after freeing slot from inventory!"));
      }
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnEquipmentChanged().Broadcast(this, addItemToPackResult);
   }
}

void ABaseHero::Unequip(const int unequipSlot) const
{
   if(const int equipId = equipment->GetEquipAtSlot(unequipSlot))
   {
      if(backpack->Count() < backpack->GetItemMax())
      {
         FMyItem                     changedEquip        = FMyItem(equipId);
         const FBackpackUpdateResult addItemToPackResult = backpack->AddItem(changedEquip);
         equipment->Unequip(unequipSlot);
         GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnEquipmentChanged().Broadcast(this, addItemToPackResult);
      } else
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Equipment", "NotEnoughSpaceUnequip", "Not enough space to unequip!"));
   }
}

void ABaseHero::OnSpellCasted(TSubclassOf<UMySpell> spellCasted)
{
   // If the spell was triggered due to using an item, then consume that item.
   if(currentItemId.GetValue())
   {
      if(backpack->FindItem(currentItemId.GetValue()) != INDEX_NONE)
      {
         const FBackpackUpdateResult itemUseBackpackUpdate = backpack->RemoveItem(FMyItem(currentItemId.GetValue()));
         GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemUsed().Broadcast(this, itemUseBackpackUpdate);
         currentItemId = INDEX_NONE;
      }
   }
}

void ABaseHero::OnInventoryItemSelected(int hIndex, int itemUsedSlotIndex)
{
   if(this->heroIndex == hIndex)
   {
      const FMyItem itemUsed = GetBackpack().GetItem(itemUsedSlotIndex);
      if(controllerRef->GetHUDManager()->IsWidgetOnScreen(EHUDs::HS_Storage))
      {
         HandleDepositItemsToStorage(itemUsedSlotIndex, itemUsed);
      } else if(controllerRef->GetHUDManager()->IsWidgetOnScreen(EHUDs::HS_Shop_General))
      {
         HandleSellItemToStore(itemUsedSlotIndex, itemUsed);
      } else
      {
         HandleInventoryItemSelected(itemUsedSlotIndex, itemUsed);
      }
   }
}

void ABaseHero::HandleInventoryItemSelected(int itemUsedSlotIndex, const FMyItem itemUsed)
{
   const FGameplayTag itemType = UItemManager::Get().GetItemInfo(itemUsed.id)->itemType;

   if(itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Equippable")))
   {
      Equip(itemUsedSlotIndex);
   } else if(itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Consumeable")))
   {
      GetHeroController()->BeginUseItem(itemUsed.id, itemUsedSlotIndex);
   } else
   {
      UE_LOG(LogTemp, Log, TEXT("ERROR WITH ITEM TYPE"));
   }
}

void ABaseHero::HandleDepositItemsToStorage(int itemUsedSlotIndex, FMyItem itemToDeposit)
{
   Cast<AStorageContainer>(GetCurrentInteractable())->GetBackpack()->TransferItems(&GetBackpack(), itemUsedSlotIndex);
   // TODO: OnItemDeposited
}

void ABaseHero::HandleSellItemToStore(int itemUsedSlotIndex, FMyItem itemToDeposit)
{
   // TODO: Calculate price and trigger some kind of event
   //Cast<AShopNPC>(GetCurrentInteractable())->

}

void ABaseHero::OnSpellLearned(TSubclassOf<UMySpell> spellLearned)
{
   --skillPoints;
}

void ABaseHero::OnSpellUpgraded(TSubclassOf<UMySpell> spellLearned)
{
   --skillPoints;
}

void ABaseHero::LoadSavedTriggers() const
{
   // TArray<FTriggerData> savedTriggers;
   // Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())->GetTriggerManager()->GetTriggerRecords().MultiFind(*GetGameName().ToString(), savedTriggers);
   // for(FTriggerData& finishedTriggerActivation : savedTriggers)
   // {
   //    controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
   // }
}

void ABaseHero::GiveItemAbilities() const
{
   // TODO: Maybe delay this to when the individual items are actually loaded?
   for(FName& id : UItemManager::Get().GetAllConsumableIDs())
   {
      TSubclassOf<UMySpell> itemAbilityClass = UItemManager::Get().GetConsumableInfo(id)->abilityClass;
      if(IsValid(itemAbilityClass))
         GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(itemAbilityClass.GetDefaultObject(), 1));
   }
}

bool ABaseHero::OnItemPickup(FMyItem& itemPickedUp) const
{
   const FBackpackUpdateResult pickupItemResult = backpack->AddItem(itemPickedUp);

   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemPickedUp().Broadcast(this, pickupItemResult);
   if(!pickupItemResult.bSuccessfulOperation)
   {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Pickup", "NoSpaceLeft", "No space in inventory to pickup everything!"));
      return false;
   }
   return true;
}

void ABaseHero::OnItemDropped(FMyItem& itemDropped, int droppedItemSlot)
{
   if(!backpack->IsEmptySlot(droppedItemSlot))
   {
      const FBackpackUpdateResult itemDropResult = backpack->EmptySlot(droppedItemSlot);
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemDropped().Broadcast(this, itemDropResult);
   } else
   {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Drop", "NoItemAtSlot", "Attempting to drop nothing..."));
   }
}

void ABaseHero::OnEquipped(int equipID, bool isEquip) const
{
   FEquipLookupRow* e = UItemManager::Get().GetEquipInfo(equipID);
   for(auto& x : e->stats.defaultAttributes)
      statComponent->ModifyStats<true>(statComponent->GetAttributeBaseValue(x.att) + x.defaultValue * (2 * isEquip - 1), x.att);

   for(auto& x : e->stats.defaultUnitScalingStats)
      statComponent->ModifyStats<true>(statComponent->GetSkillBaseValue(x.stat) + x.defaultValue * (2 * isEquip - 1), x.stat);

   for(auto& x : e->stats.defaultVitals)
      statComponent->ModifyStats<true>(statComponent->GetVitalBaseValue(x.vit) + x.defaultValue * (2 * isEquip - 1), x.vit);

   for(auto& x : e->stats.defaultMechanics)
      statComponent->ModifyStats<true>(statComponent->GetMechanicBaseValue(x.mech) + x.defaultValue * (2 * isEquip - 1), x.mech);
}

void ABaseHero::SetSelected(bool value)
{
   Super::SetSelected(value);
   if(value)
   {
      controllerRef->GetBasePlayer()->selectedHeroes.AddUnique(this);
   } else
   {
      controllerRef->GetBasePlayer()->selectedHeroes.RemoveSingle(this);
   }
}
