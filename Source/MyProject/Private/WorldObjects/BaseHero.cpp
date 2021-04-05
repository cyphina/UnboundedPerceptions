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
<<<<<<< HEAD
#include "HeroInventory.h"
#include "PartyDelegateStore.h"
=======
#include "GameplayDelegateContext.h"
#include "HeroInventory.h"
#include "ItemDelegateContext.h"
#include "PartyDelegateContext.h"
>>>>>>> componentrefactor
#include "RTSIngameWidget.h"
#include "RTSSidebarWidget.h"
#include "TargetComponent.h"
#include "UpStatComponent.h"
#include "Components/SpellCastComponent.h"
#include "SpellSystem/MySpell.h"
#include "SpellSystem/Spellbook.h"
#include "WorldObjects/NPC.h"
<<<<<<< HEAD
#include "Items/ItemDelegateStore.h"
=======
#include "ItemDelegateContext.h"
#include "ShopNPC.h"
#include "SpellDelegateStore.h"
#include "StorageContainer.h"
#include "UIDelegateContext.h"
>>>>>>> componentrefactor

ABaseHero::ABaseHero(const FObjectInitializer& oI) : AAlly(oI)
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
<<<<<<< HEAD
   equipment->OnEquipmentChanged().BindUObject(this, &ABaseHero::OnEquipped);
=======
   equipment->OnEquipmentContainerChanged().BindUObject(this, &ABaseHero::OnEquipped);
>>>>>>> componentrefactor

   GetHeroController()->GetSpellCastComponent()->OnSpellCasted().AddUObject(this, &ABaseHero::OnSpellCasted);

   LoadSavedTriggers();
   GiveItemAbilities();
<<<<<<< HEAD

   spellbook = USpellBook::CreateSpellBook(this);
   spellbook->OnSpellLearned().AddUObject(this, &ABaseHero::OnSpellLearned);
   spellbook->OnSpellUpgraded().AddUObject(this, &ABaseHero::OnSpellUpgraded);

   OnPickupItem().BindUObject(this, &ABaseHero::OnItemPickup);
=======

   spellbook = USpellBook::CreateSpellBook(this, spellbookClass);

   if(spellbook)
   {
      SpellGameContext::OnSpellLearnedEvent.AddUObject(this, &ABaseHero::OnSpellLearned);
      SpellGameContext::OnSpellUpgradedEvent.AddUObject(this, &ABaseHero::OnSpellUpgraded);
   }

   if(IsEnabled())
   {
      GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
         SetEnabled(true);
      });
   }
>>>>>>> componentrefactor
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
<<<<<<< HEAD
=======
   Super::UnPossessed();
>>>>>>> componentrefactor
   OnUnitDie().RemoveAll(this);
}

void ABaseHero::SetEnabled(bool bEnabled)
{
   Super::SetEnabled(bEnabled);
<<<<<<< HEAD
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateStore>()->OnHeroActiveChanged().Broadcast(this, bEnabled);
   if(bEnabled) {
      controllerRef->GetHUDManager()->GetIngameHUD()->GetInventoryHUD()->OnItemSelected().AddUObject(this, &ABaseHero::OnItemSelected);
   } else {
      controllerRef->GetHUDManager()->GetIngameHUD()->GetInventoryHUD()->OnItemSelected().RemoveAll(this);
=======
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnHeroActiveChanged().Broadcast(this, bEnabled);
   if(bEnabled)
   {
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().AddUObject(this,
                                                                                                                                  &ABaseHero::OnAttributePointAllocated);
      OnPickupItem().BindUObject(this, &ABaseHero::OnItemPickup);
   }
   else
   {
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().RemoveAll(this);
      OnPickupItem().Unbind();
>>>>>>> componentrefactor
   }
}

void ABaseHero::CheckGameOverOnDeath() const
{
   bool isAllDead = true;
<<<<<<< HEAD
   for(const ABaseHero* hero : controllerRef->GetBasePlayer()->GetHeroes()) {
      if(!hero->GetCombatInfo()->isDead) {
=======
   for(const ABaseHero* hero : controllerRef->GetBasePlayer()->GetHeroes())
   {
      if(!hero->GetCombatInfo()->isDead)
      {
>>>>>>> componentrefactor
         isAllDead = false;
         break;
      }
   }
<<<<<<< HEAD
   if(isAllDead) controllerRef->GetGameMode()->GameOver();
=======
   if(isAllDead)
   {
      controllerRef->GetGameMode()->GameOver();
   }
>>>>>>> componentrefactor
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
<<<<<<< HEAD
      expForLevel *= NEXT_EXP_MULTIPLIER; //
=======
>>>>>>> componentrefactor
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
<<<<<<< HEAD
   OnLevelUpEvent.Broadcast();
}

void ABaseHero::SetCurrentInteractable(AActor* newInteractable)
{
   currentInteractable = newInteractable;
   GetTargetComponent()->SetTarget(newInteractable);
=======
   expForLevel *= NEXT_EXP_MULTIPLIER;
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnHeroLevelUp().Broadcast(this);
>>>>>>> componentrefactor
}

void ABaseHero::SetCurrentInteractable(AActor* newInteractable)
{
<<<<<<< HEAD
   if(isIncrementing) {
      GetAbilitySystemComponent()->SetNumericAttributeBase(UMyAttributeSet::IndexAtts(static_cast<int>(att)), statComponent->GetAttributeBaseValue(att) + 1);
      --attPoints;
      statComponent->UpdateStats(UMyAttributeSet::IndexAtts(static_cast<int>(att)));
   } else {
      GetAbilitySystemComponent()->SetNumericAttributeBase(UMyAttributeSet::IndexAtts(static_cast<int>(att)), statComponent->GetAttributeBaseValue(att) - 1);
      ++attPoints;
      statComponent->UpdateStats(UMyAttributeSet::IndexAtts(static_cast<int>(att)));
   }
}

void ABaseHero::Equip(const int backpackIndex)
{
   const int prevEquipInSlot = equipment->Equip(backpack->GetItem(backpackIndex).id);
   backpack->RemoveItemAtSlot(backpackIndex);

   if(prevEquipInSlot) {
      auto item{FMyItem(prevEquipInSlot)};
      if(!ensure(backpack->AddItem(item))) UE_LOG(LogTemp, Error, TEXT("Impossible case reached no space to swap equipment"));
   }

   ItemChangeEvents::OnEquipmentChangedEvent.Broadcast(this, backpack->GetItem(backpackIndex));
=======
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
         GetStatComponent()->UpdateStats(URTSAttributeSet::IndexAtts(static_cast<int>(att)));
      }
      else
      {
         GetStatComponent()->ModifyStats<true>(statComponent->GetAttributeBaseValue(att) - 1, att);
         ++attPoints;
         GetStatComponent()->UpdateStats(URTSAttributeSet::IndexAtts(static_cast<int>(att)));
      }
   }
}

void ABaseHero::Equip(const int backpackIndex)
{
   TArray<int>                 updatedSlots;
   const int                   prevEquipInSlot           = equipment->Equip(backpack->GetItem(backpackIndex).id);
   const FBackpackUpdateResult removeEquipFromPackResult = backpack->RemoveItemAtSlot(backpackIndex);
   updatedSlots.Append(removeEquipFromPackResult.updatedBackpackIndices);

   if(prevEquipInSlot)
   {
      const FBackpackUpdateResult addItemToPackResult = backpack->AddItem(FMyItem(prevEquipInSlot));
      updatedSlots.Append(addItemToPackResult.updatedBackpackIndices);
      if(!ensure(addItemToPackResult.bSuccessfulOperation))
      {
         UE_LOG(LogTemp, Error, TEXT("Impossible case reached. No space to swap equipment even after freeing slot from inventory!"));
      }
   }
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnEquipmentChanged().Broadcast(this, updatedSlots);
>>>>>>> componentrefactor
}

void ABaseHero::Unequip(const int unequipSlot) const
{
<<<<<<< HEAD
   if(const int equipId = equipment->GetEquipAtSlot(unequipSlot)) {
      if(backpack->Count() < backpack->GetItemMax()) {
         FMyItem   changedEquip = FMyItem(equipId);
         backpack->AddItem(changedEquip);
         equipment->Unequip(unequipSlot);
         ItemChangeEvents::OnEquipmentChangedEvent.Broadcast(this, changedEquip);
      } else
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Equipment", "NotEnoughSpaceUnequip", "Not enough space to unequip!"));
=======
   if(const int equipId = equipment->GetEquipAtSlot(unequipSlot))
   {
      if(backpack->Count() < backpack->GetItemMax())
      {
         const FMyItem               changedEquip        = FMyItem(equipId);
         const FBackpackUpdateResult addItemToPackResult = backpack->AddItem(changedEquip);
         equipment->Unequip(unequipSlot);
         GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnEquipmentChanged().Broadcast(
             this, addItemToPackResult.updatedBackpackIndices);
      }
      else
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Equipment", "NotEnoughSpaceUnequip", "Not enough space to unequip!"));
   }
}

void ABaseHero::OnSpellCasted(TSubclassOf<UMySpell> spellCasted)
{
   // If the spell was triggered due to using an item, then consume that item.
   if(currentItemId.IsSet())
   {
      if(backpack->FindItem(currentItemId.GetValue()) != INDEX_NONE)
      {
         const FBackpackUpdateResult itemUseBackpackUpdate = backpack->RemoveItem(FMyItem(currentItemId.GetValue()));
         GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UItemDelegateContext>()->OnItemUsed().Broadcast(this, itemUseBackpackUpdate);
         currentItemId = INDEX_NONE;
      }
>>>>>>> componentrefactor
   }
}

void ABaseHero::OnSpellLearned(const ABaseHero& heroThatLearnedSpell, TSubclassOf<UMySpell> spellLearned)
{
<<<<<<< HEAD
   equipment->SwapEquips(equipSlot1, equipSlot2);
   ItemChangeEvents::OnEquipmentChangedEvent.Broadcast(this, FMyItem(-1, 0));
}

void ABaseHero::OnSpellCasted(TSubclassOf<UMySpell> spellCasted)
{
   // If the spell was triggered due to using an item, then consume that item.
   if(currentItemId.GetValue()) {
      if(backpack->FindItem(currentItemId.GetValue()) != INDEX_NONE) { backpack->RemoveItem(FMyItem(currentItemId.GetValue())); }
   }
}

void ABaseHero::OnItemSelected(int hIndex, int itemUsedSlotIndex)
{
   const FMyItem      itemUsed = GetBackpack().GetItem(itemUsedSlotIndex);
   const FGameplayTag itemType = UItemManager::Get().GetItemInfo(itemUsed.id)->itemType;

   if(this->heroIndex == hIndex) {
      if(itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Equippable"))) {
         Equip(itemUsedSlotIndex);
         ItemChangeEvents::OnItemUsedEvent.Broadcast(this, itemUsed);
      } else if(itemType.MatchesTag(UGameplayTagsManager::Get().RequestGameplayTag("Item.Consumeable"))) {
         GetHeroController()->BeginUseItem(itemUsed.id, itemUsedSlotIndex);
         ItemChangeEvents::OnItemUsedEvent.Broadcast(this, itemUsed);
      } else {
         UE_LOG(LogTemp, Log, TEXT("ERROR WITH ITEM TYPE"));
      }
   }
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
   TArray<FTriggerData> savedTriggers;
   Cast<ARTSGameMode>(GetWorld()->GetAuthGameMode())->GetTriggerManager()->GetTriggerRecords().MultiFind(*GetGameName().ToString(), savedTriggers);
   for(FTriggerData& finishedTriggerActivation : savedTriggers) {
      controllerRef->GetGameMode()->GetTriggerManager()->ActivateTrigger(finishedTriggerActivation);
   }
}

void ABaseHero::GiveItemAbilities() const
{
   // TODO: Maybe delay this to when the individual items are actually loaded?
   for(FName& id : UItemManager::Get().GetAllConsumableIDs()) {
      TSubclassOf<UMySpell> itemAbilityClass = UItemManager::Get().GetConsumableInfo(id)->abilityClass;
      if(IsValid(itemAbilityClass)) GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(itemAbilityClass.GetDefaultObject(), 1));
   }
}

bool ABaseHero::OnItemPickup(FMyItem& itemPickedUp) const
{
   if(!backpack->AddItem(itemPickedUp)) {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Pickup", "NoSpaceLeft", "No space in inventory to pickup everything!"));
      return false;
   }
   ItemChangeEvents::OnItemPickedUpEvent.Broadcast(this, itemPickedUp);
   return true;
}

void ABaseHero::OnItemDropped(FMyItem& itemDropped, int droppedItemSlot)
{
   if(!backpack->IsEmptySlot(droppedItemSlot)) {
      backpack->RemoveItemAtSlot(droppedItemSlot, itemDropped.count);
      ItemChangeEvents::OnItemDroppedEvent.Broadcast(this, itemDropped);
   } else {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Drop", "NoItemAtSlot", "Attempting to drop nothing..."));
   }
}

=======
   if(&heroThatLearnedSpell == this)
   {
      --skillPoints;
   }
}

void ABaseHero::OnSpellUpgraded(const ABaseHero& heroThatLearnedSpell, TSubclassOf<UMySpell> spellLearned)
{
   if(&heroThatLearnedSpell == this)
   {
      --skillPoints;
   }
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
      if(IsValid(itemAbilityClass)) GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(itemAbilityClass.GetDefaultObject(), 1));
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
   }
   else
   {
      URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NSLOCTEXT("Drop", "NoItemAtSlot", "Attempting to drop nothing..."));
   }
}

>>>>>>> componentrefactor
void ABaseHero::OnEquipped(int equipID, bool isEquip) const
{
   FEquipLookupRow* e = UItemManager::Get().GetEquipInfo(equipID);
   for(auto& x : e->stats.defaultAttributes)
<<<<<<< HEAD
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
   if(value) {
      controllerRef->GetBasePlayer()->selectedHeroes.AddUnique(this);
   } else {
      controllerRef->GetBasePlayer()->selectedHeroes.RemoveSingle(this);
   }
}
=======
   {
      statComponent->ModifyStats<true>(statComponent->GetAttributeBaseValue(x.att) + x.defaultValue * (2 * isEquip - 1), x.att);
   }

   for(auto& x : e->stats.defaultUnitScalingStats)
   {
      statComponent->ModifyStats<true>(statComponent->GetSkillBaseValue(x.stat) + x.defaultValue * (2 * isEquip - 1), x.stat);
   }

   for(auto& x : e->stats.defaultVitals)
   {
      statComponent->ModifyStats<true>(statComponent->GetVitalBaseValue(x.vit) + x.defaultValue * (2 * isEquip - 1), x.vit);
   }

   for(auto& x : e->stats.defaultMechanics)
   {
      statComponent->ModifyStats<true>(statComponent->GetMechanicBaseValue(x.mech) + x.defaultValue * (2 * isEquip - 1), x.mech);
   }
}

void ABaseHero::SetUnitSelected(bool value)
{
   Super::SetUnitSelected(value);
   if(value)
   {
      controllerRef->GetBasePlayer()->AddSelectedHero(this);
   }
   else
   {
      controllerRef->GetBasePlayer()->RemoveSelectedHero(this);
   }
}
>>>>>>> componentrefactor
