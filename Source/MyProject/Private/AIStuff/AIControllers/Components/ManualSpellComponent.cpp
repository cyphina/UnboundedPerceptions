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
#include "GameplayDelegateContext.h"
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "SpellFunctionLibrary.h"
#include "SpellTargetingTypes.h"
#include "TargetComponent.h"
#include "UIDelegateContext.h"
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
   if(URTSAbilitySystemComponent* abilityComp = GetOwner()->FindComponentByClass<URTSAbilitySystemComponent>())
   {
      const bool spellSelected = PressedCastSpell(abilityComp->GetAbilities()[spellCastingIndex]);
      return spellSelected;
   }
   return false;
}

bool UManualSpellComponent::PressedCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();
   if(IsValid(spell))
   {
      if(AUserInput* CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
      {
         if(!spell->IsOnCD(Cast<AUnit>(GetOwner())->GetAbilitySystemComponent()))
         {
            if(spell->GetCost(unitOwner->GetAbilitySystemComponent()) <= unitOwner->GetStatComponent()->GetVitalCurValue(EVitals::Mana))
            {
               if(currentlySelectedSpell == spellToCast)
               {
                  DeselectSpell();
                  return false;
               }

               if(spell->GetTargeting()->GetClass() == TSubclassOf<UUpSpellTargeting_None>())
               {
                  unitOwner->GetUnitController()->Stop();
                  unitOwner->GetUnitController()->FindComponentByClass<USpellCastComponent>()->IncantationCheck(spellToCast);
               } else
               {
                  CPCRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Magic);
               }

               currentlySelectedSpell = spellToCast;

               if(spell->GetTargeting()->GetClass() == TSubclassOf<UUpSpellTargeting_Area>())
               {
                  // TODO: depending on the spell area targeting, use different indicators
                  CPCRef->GetCameraPawn()->ShowSpellCircle(spell->GetAOE(unitOwner->GetAbilitySystemComponent()));
               } else
               {
                  CPCRef->GetCameraPawn()->HideSpellCircle();
               }

               return true;
            }
            URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NOT_ENOUGH_MANA_TEXT);
            return false;
         } // If spell still not ready
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), ON_COOLDOWN_TEXT);
         return false;
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

void UManualSpellComponent::OnSkillActivated(int spellIndex)
{
   if(unitOwner->IsSelected())
   {
      if(spellIndex >= 0 && spellIndex < unitOwner->GetAbilitySystemComponent()->GetAbilities().Num()) { PressedCastSpell(spellIndex); }
   }
}

void UManualSpellComponent::OnSkillSlotDropped(int dragSlotIndex, int dropSlotIndex)
{
   if(AUserInput* CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      if(ABasePlayer* basePlayer = CPCRef->GetBasePlayer())
      {
         if(AUnit* focusedUnit = basePlayer->GetFocusedUnit())
         {
            if(focusedUnit == unitOwner)
            {
               URTSAbilitySystemComponent* focusedABComp = unitOwner->GetAbilitySystemComponent();
               const TSubclassOf<UMySpell> droppedSpell  = focusedABComp->GetSpellAtSlot(dropSlotIndex);

               focusedABComp->SetSpellAtSlot(focusedABComp->GetSpellAtSlot(dragSlotIndex), dropSlotIndex);
               focusedABComp->SetSpellAtSlot(droppedSpell, dragSlotIndex);
            }
         }
      }
   }
}

bool UManualSpellComponent::SetupSpellTargeting(UPARAM(ref) FHitResult& hitResult)
{
   if(IsValid(currentlySelectedSpell) && hitResult.IsValidBlockingHit())
   {
      const auto spellTargeting = currentlySelectedSpell.GetDefaultObject()->GetTargeting().GetDefaultObject();
      if(spellTargeting->ManualTargetingCheck(unitOwner, hitResult))
      {
         spellTargeting->ClickResponse(hitResult, currentlySelectedSpell, *this);
         return true;
      }
   }
   return false;
}

void UManualSpellComponent::BeginPlay()
{
   Super::BeginPlay();
   unitOwner = Cast<AUnit>(Cast<AUnitController>(GetOwner())->GetUnitOwner());
   unitOwner->GetUnitController()->OnUnitStopped().AddUObject(this, &UManualSpellComponent::OnUnitStopped);
   spellCastComp = GetOwner()->FindComponentByClass<USpellCastComponent>();
   if(ensure(spellCastComp))
   {
      spellCastComp->OnSpellCasted().AddUObject(this, &UManualSpellComponent::OnSpellCasted);
   }

   if(const AUserInput* userInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      userInput->GetLocalPlayer()->GetSubsystem<UGameplayDelegateContext>()->OnSkillActivated().AddUObject(this, &UManualSpellComponent::OnSkillActivated);
      userInput->GetLocalPlayer()->GetSubsystem<UUIDelegateContext>()->OnSkillSlotDroppedEvent.AddDynamic(this, &UManualSpellComponent::OnSkillSlotDropped);
   }
}

void UManualSpellComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
   Super::EndPlay(EndPlayReason);
   if(AUserInput* userInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      userInput->GetLocalPlayer()->GetSubsystem<UGameplayDelegateContext>()->OnSkillActivated().RemoveAll(this);
   }
}

void UManualSpellComponent::DeselectSpell()
{
   if(AUserInput* cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      currentlySelectedSpell = nullptr;
      cpcRef->GetCameraPawn()->HideSpellCircle();
      cpcRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Select);
   }
}

void UManualSpellComponent::OnSpellCasted(TSubclassOf<UMySpell> spellCasted)
{
   if(AUserInput* cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      if(cpcRef->GetBasePlayer()->GetFocusedUnit() == GetOwner())
      {
         const int spellSlotIndex = unitOwner->GetAbilitySystemComponent()->FindSlotIndexOfSpell(spellCasted);
         if(spellSlotIndex != INDEX_NONE) { SpellHUDEvents::OnFocusedUnitSpellCastedEvent.Broadcast(unitOwner, spellSlotIndex); }
      }
   }
}

bool UManualSpellComponent::IsTargetingSelf()
{
   return unitOwner->GetTargetComponent()->GetTargetUnit() == unitOwner;
}

void UManualSpellComponent::FinalizeSpellTargeting(const UUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult)
{
   if(AUserInput* userInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      userInput->GetCameraPawn()->HideSpellCircle();
      userInput->GetCameraPawn()->SetSecondaryCursor(); // Just set cursor to to select so the cursor check loop will quickly change the cursor back to normal
      spellTargeting->ManualSetSpellTarget(unitOwner->GetTargetComponent(), hitResult);
      spellCastComp->BeginCastSpell(spellClass);
   }
}
