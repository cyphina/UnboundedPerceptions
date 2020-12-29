// Created 8/21/20 12:17 PM

#include "MyProject.h"
#include "ManualSpellComponent.h"
#include "MySpell.h"
#include "BaseCharacter.h"
#include "ECursorStates.h"
#include "SpellSystem/SpellDelegateStore.h"
#include "BasePlayer.h"
#include "DefaultCursorClickFunctionality.h"
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "RTSStateComponent.h"
#include "SpellTargetingTypes.h"
#include "TargetComponent.h"
#include "Unit.h"
#include "UnitController.h"
#include "UpStatComponent.h"
#include "UserInput.h"

bool UManualSpellComponent::PressedCastSpell(int spellCastingIndex)
{
   if(URTSAbilitySystemComponent* abilityComp = GetOwner()->FindComponentByClass<URTSAbilitySystemComponent>()) {
      const bool spellSelected = PressedCastSpell(abilityComp->GetAbilities()[spellCastingIndex]);
      return spellSelected;
   }
   return false;
}

bool UManualSpellComponent::PressedCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();
   if(IsValid(spell)) {
      if(AUserInput* CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())) {
         if(!spell->IsOnCD(Cast<AUnit>(GetOwner())->GetAbilitySystemComponent())) {
            if(spell->GetCost(unitOwner->GetAbilitySystemComponent()) <= unitOwner->GetStatComponent()->GetVitalCurValue(EVitals::Mana)) {
               if(currentlySelectedSpell == spellToCast) {
                  DeselectSpell();
                  return false;
               }

               if(spell->GetTargeting()->GetTargetTag().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.None"))) {
                  unitOwner->GetUnitController()->Stop();
                  SetupDelayedSpellProps(spellToCast);
                  unitOwner->GetUnitController()->FindComponentByClass<USpellCastComponent>()->IncantationCheck(spellToCast);
               } else {
                  CPCRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Magic);
               }

               // Set our current spell to spellToCast for recording purposes
               currentlySelectedSpell = spellToCast;

               if(spell->GetTargeting()->GetTargetTag().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area"))) {
                  // TODO: depending on the spell area targeting, use different indicators
                  CPCRef->GetCameraPawn()->ShowSpellCircle(spell->GetAOE(unitOwner->GetAbilitySystemComponent()));
               } else {
                  CPCRef->GetCameraPawn()->HideSpellCircle();
               }

               return true;
            } else {
               URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NOT_ENOUGH_MANA_TEXT);
               return false;
            }
         } else // If spell still not ready
         {
            URTSIngameWidget::NativeDisplayHelpText(GetWorld(), ON_COOLDOWN_TEXT);
            return false;
         }
      }
   }
   return false;
}

bool UManualSpellComponent::InvalidTarget() const
{
   URTSIngameWidget::NativeDisplayHelpText(GetWorld(), INVALID_TARGET_TEXT);
   return false;
}

void UManualSpellComponent::SetupDelayedSpellProps(TSubclassOf<UMySpell> spellToCast) const
{
   spellCastComp->currentSpell = spellToCast;
}

void UManualSpellComponent::OnUnitStopped()
{
   currentlySelectedSpell = nullptr;
}

bool UManualSpellComponent::SetupSpellTargeting(UPARAM(ref) FHitResult& hitResult)
{
   if(IsValid(currentlySelectedSpell) && hitResult.IsValidBlockingHit()) {
      const auto spellTargeting = currentlySelectedSpell.GetDefaultObject()->GetTargeting();
      if(spellTargeting->ManualTargetingCheck(hitResult)) {
         spellTargeting->ClickResponse(hitResult, currentlySelectedSpell, *this);
         return true;
      }
   }
   return false;
}

void UManualSpellComponent::BeginPlay()
{
   unitOwner = Cast<AUnit>(Cast<AUnitController>(GetOwner())->GetUnitOwner());
   unitOwner->GetUnitController()->OnUnitStopped().AddUObject(this, &UManualSpellComponent::OnUnitStopped);
   GetOwner()->FindComponentByClass<USpellCastComponent>()->OnSpellCasted().AddUObject(this, &UManualSpellComponent::OnSpellCasted);
   SpellHUDEvents::OnSpellSlotReplacedEvent.BindUObject(this, &UManualSpellComponent::OnSpellSlotReplaced);
}

void UManualSpellComponent::DeselectSpell()
{
   if(AUserInput* cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())) {
      currentlySelectedSpell = nullptr;
      cpcRef->GetCameraPawn()->HideSpellCircle();
      cpcRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Select);
   }
}

void UManualSpellComponent::OnSpellCasted(TSubclassOf<UMySpell> spellCasted)
{
   if(AUserInput* cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())) {
      if(cpcRef->GetBasePlayer()->GetFocusedUnit() == GetOwner()) {
         if(AUnit* unitOwner = Cast<AUnitController>(GetOwner())->GetUnitOwner()) {
            if(const int spellSlotIndex = unitOwner->GetAbilitySystemComponent()->FindSlotIndexOfSpell(spellCasted); spellSlotIndex != INDEX_NONE) {
               SpellHUDEvents::OnFocusedUnitSpellCastedEvent.Broadcast(unitOwner, spellSlotIndex);
            }
         }
      }
   }
}

void UManualSpellComponent::OnSpellSlotReplaced(int dropSlotindex, TSubclassOf<UMySpell> spellClass)
{
   unitOwner->GetAbilitySystemComponent()->SetSpellAtSlot(spellClass, dropSlotindex);
}

void UManualSpellComponent::FinalizeSpellTargetingOnSelf(const FUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult&)
{
   Cast<AUnitController>(GetOwner())->Stop();
   SetupDelayedSpellProps(spellClass);
   spellCastComp->IncantationCheck(spellClass);
   Cast<AUserInput>(GetWorld()->GetFirstPlayerController())->GetCameraPawn()->SetSecondaryCursor();
}

bool UManualSpellComponent::IsTargetingSelf()
{
   return unitOwner->GetTargetComponent()->GetTargetUnit() == unitOwner;
}

void UManualSpellComponent::FinalizeSpellTargeting(const FUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult)
{
   if(AUserInput* userInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())) {
      userInput->GetCameraPawn()->HideSpellCircle();
      unitOwner->GetUnitController()->Stop();
      spellTargeting->ManualSetSpellTarget(unitOwner->GetTargetComponent(), hitResult);
      userInput->GetCameraPawn()->SetSecondaryCursor(); // Just set cursor to to select so the cursor check loop will quickly change the cursor back to normal
      SetupDelayedSpellProps(spellClass);               // Reassign these variables that were cleared since we had to stop our current action
      spellTargeting->ManualAdjustCastPosition(spellCastComp, spellClass, hitResult);
   }
}
