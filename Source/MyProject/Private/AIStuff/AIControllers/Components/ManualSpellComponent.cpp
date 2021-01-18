// Created 8/21/20 12:17 PM

#include "MyProject.h"
#include "ManualSpellComponent.h"
#include "MySpell.h"
#include "BaseCharacter.h"
#include "ECursorStates.h"
#include "SpellDelegateStore.h"
#include "BasePlayer.h"
#include "DefaultCursorClickFunctionality.h"
#include "NoTargeting.h"
#include "AOETargeting.h"
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "SpellTargetingTypes.h"
#include "TargetComponent.h"
#include "Unit.h"
#include "UnitController.h"
#include "UpStatComponent.h"
#include "UserInput.h"

class UUpSpellTargeting_Area;
const FText UManualSpellComponent::NOT_ENOUGH_MANA_TEXT = NSLOCTEXT("HelpMessages", "Mana", "Not Enough Mana!");
const FText UManualSpellComponent::INVALID_TARGET_TEXT  = NSLOCTEXT("HelpMessages", "Target", "Invalid Target!");
const FText UManualSpellComponent::ON_COOLDOWN_TEXT     = NSLOCTEXT("HelpMessages", "CD", "Spell is on Cooldown!");

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

               if(spell->GetTargeting()->GetClass() == TSubclassOf<UUpSpellTargeting_None>()) {
                  unitOwner->GetUnitController()->Stop();
                  unitOwner->GetUnitController()->FindComponentByClass<USpellCastComponent>()->IncantationCheck(spellToCast);
               } else {
                  CPCRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Magic);
               }

               currentlySelectedSpell = spellToCast;

               if(spell->GetTargeting()->GetClass() == TSubclassOf<UUpSpellTargeting_Area>()) {
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

void UManualSpellComponent::OnUnitStopped()
{
   currentlySelectedSpell = nullptr;
}

void UManualSpellComponent::OnSkillSlotPressed(int spellIndex)
{
   if(unitOwner->IsSelected())
   {
      if(spellIndex >= 0 && spellIndex < unitOwner->GetAbilitySystemComponent()->GetAbilities().Num())
      {
         PressedCastSpell(spellIndex);
      }
   }
}

bool UManualSpellComponent::SetupSpellTargeting(UPARAM(ref) FHitResult& hitResult)
{
   if(IsValid(currentlySelectedSpell) && hitResult.IsValidBlockingHit()) {
      const auto spellTargeting = currentlySelectedSpell.GetDefaultObject()->GetTargeting().GetDefaultObject();
      if(spellTargeting->ManualTargetingCheck(unitOwner, hitResult)) {
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
   Cast<AUserInput>(GetWorld()->GetFirstPlayerController())->GetCameraPawn()->OnSkillSlotPressed().AddUObject(this, &UManualSpellComponent::OnSkillSlotPressed);
}

void UManualSpellComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
   Cast<AUserInput>(GetWorld()->GetFirstPlayerController())->GetCameraPawn()->OnSkillSlotPressed().RemoveAll(this);
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
         const int spellSlotIndex = unitOwner->GetAbilitySystemComponent()->FindSlotIndexOfSpell(spellCasted);
         if(spellSlotIndex != INDEX_NONE) {
            SpellHUDEvents::OnFocusedUnitSpellCastedEvent.Broadcast(unitOwner, spellSlotIndex);
         }
      }
   }
}

bool UManualSpellComponent::IsTargetingSelf()
{
   return unitOwner->GetTargetComponent()->GetTargetUnit() == unitOwner;
}

void UManualSpellComponent::FinalizeSpellTargeting(const UUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult)
{
   if(AUserInput* userInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())) {
      userInput->GetCameraPawn()->HideSpellCircle();
      userInput->GetCameraPawn()->SetSecondaryCursor(); // Just set cursor to to select so the cursor check loop will quickly change the cursor back to normal
      spellTargeting->ManualSetSpellTarget(unitOwner->GetTargetComponent(), hitResult);
      spellCastComp->BeginCastSpell(spellClass);
   }
}
