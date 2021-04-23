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

const float ABaseHero::NEXT_EXP_MULTIPLIER = 1.5f;

ABaseHero::ABaseHero(const FObjectInitializer& oI) : AAlly(oI)
{
}

void ABaseHero::BeginPlay()
{
   Super::BeginPlay();

   currentInteractable = nullptr;

   backpack = UBackpack::CreateBackpack(this, 40);

   equipment = NewObject<UEquipmentContainer>(this, "Equipment");
   equipment->OnEquipmentContainerChanged().BindUObject(this, &ABaseHero::OnEquipped);

   LoadSavedTriggers();

   if(IsEnabled())
   {
      GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
         SetEnabled(true);
      });
   }
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

   if(!HasAuthority() || !IsRunningDedicatedServer())
   {
      if(controllerRef)
      {
         player = Cast<ABasePlayer>(controllerRef->PlayerState);
         if(player)
         {
            player->allHeroes.Add(this);
         }
      }

      heroController = Cast<AHeroAIController>(GetController());
      OnUnitDie().AddUObject(this, &ABaseHero::CheckGameOverOnDeath);
      GetHeroController()->GetSpellCastComponent()->OnSpellCasted().AddUObject(this, &ABaseHero::OnSpellCasted);

      if(HasAuthority() || !IsRunningDedicatedServer())
      {
         GiveItemAbilities();

         spellbook = USpellBook::CreateSpellBook(this, spellbookClass);

         if(spellbook)
         {
            SpellGameContext::OnSpellLearnedEvent.AddUObject(this, &ABaseHero::OnSpellLearned);
            SpellGameContext::OnSpellUpgradedEvent.AddUObject(this, &ABaseHero::OnSpellUpgraded);
         }
      }
   }
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
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().AddUObject(this,
                                                                                                                                  &ABaseHero::OnAttributePointAllocated);
      OnPickupItem().BindUObject(this, &ABaseHero::OnItemPickup);
   }
   else
   {
      GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UUIDelegateContext>()->OnAttributePointAllocated().RemoveAll(this);
      OnPickupItem().Unbind();
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
   {
      controllerRef->GetGameMode()->GameOver();
   }
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
   expForLevel *= NEXT_EXP_MULTIPLIER;
   GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UPartyDelegateContext>()->OnHeroLevelUp().Broadcast(this);
}

APlayerController* ABaseHero::GetOwningPlayer() const
{
   return controllerRef;
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
}

void ABaseHero::Unequip(const int unequipSlot) const
{
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
   }
}

void ABaseHero::OnSpellLearned(const ABaseHero& heroThatLearnedSpell, TSubclassOf<UMySpell> spellLearned)
{
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

void ABaseHero::OnItemDropped(int droppedItemSlot)
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

void ABaseHero::OnEquipped(int equipID, bool isEquip) const
{
   FEquipLookupRow* e = UItemManager::Get().GetEquipInfo(equipID);
   for(auto& x : e->stats.defaultAttributes)
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
      controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnHeroSelectedDelegate.Broadcast(this);
   }
   else
   {
      controllerRef->GetBasePlayer()->RemoveSelectedHero(this);
      controllerRef->GetLocalPlayer()->GetSubsystem<UPartyDelegateContext>()->OnHeroDeselectedDelegate.Broadcast(this);
   }
}
