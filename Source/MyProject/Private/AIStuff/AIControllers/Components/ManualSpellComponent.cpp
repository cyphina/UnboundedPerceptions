// Created 8/21/20 12:17 PM

#include "MyProject.h"
#include "ManualSpellComponent.h"
#include "MySpell.h"
#include "BaseCharacter.h"
#include "ECursorStates.h"
<<<<<<< HEAD
#include "SpellSystem/SpellDelegateStore.h"
#include "BasePlayer.h"
#include "DefaultCursorClickFunctionality.h"
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "SpellTargetingTypes.h"
#include "TargetComponent.h"
=======
#include "SpellDelegateStore.h"
#include "BasePlayer.h"
#include "DefaultCursorClickFunctionality.h"
#include "NoTargeting.h"
#include "AOETargeting.h"
#include "GameplayDelegateContext.h"
#include "RTSIngameWidget.h"
#include "RTSPawn.h"
#include "SpellCastComponent.h"
#include "SpellDataLibrary.h"
#include "SpellTargetingTypes.h"
#include "TargetComponent.h"
#include "UIDelegateContext.h"
>>>>>>> componentrefactor
#include "Unit.h"
#include "UnitController.h"
#include "UpStatComponent.h"
#include "UserInput.h"

<<<<<<< HEAD
=======
class UUpSpellTargeting_Area;
>>>>>>> componentrefactor
const FText UManualSpellComponent::NOT_ENOUGH_MANA_TEXT = NSLOCTEXT("HelpMessages", "Mana", "Not Enough Mana!");
const FText UManualSpellComponent::INVALID_TARGET_TEXT  = NSLOCTEXT("HelpMessages", "Target", "Invalid Target!");
const FText UManualSpellComponent::ON_COOLDOWN_TEXT     = NSLOCTEXT("HelpMessages", "CD", "Spell is on Cooldown!");

bool UManualSpellComponent::PressedCastSpell(int spellCastingIndex)
{
<<<<<<< HEAD
   if(URTSAbilitySystemComponent* abilityComp = GetOwner()->FindComponentByClass<URTSAbilitySystemComponent>()) {
=======
   if(URTSAbilitySystemComponent* abilityComp = GetRTSAbilityComp())
   {
>>>>>>> componentrefactor
      const bool spellSelected = PressedCastSpell(abilityComp->GetAbilities()[spellCastingIndex]);
      return spellSelected;
   }
   return false;
}

bool UManualSpellComponent::PressedCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();
<<<<<<< HEAD
   if(IsValid(spell)) {
      if(AUserInput* CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())) {
         if(!spell->IsOnCD(Cast<AUnit>(GetOwner())->GetAbilitySystemComponent())) {
            if(spell->GetCost(unitOwner->GetAbilitySystemComponent()) <= unitOwner->GetStatComponent()->GetVitalCurValue(EVitals::Mana)) {
               if(currentlySelectedSpell == spellToCast) {
=======
   if(IsValid(spell))
   {
      if(AUserInput* CPCRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
      {
         if(!spell->IsOnCD(GetRTSAbilityComp()))
         {
            if(spell->GetCost(GetRTSAbilityComp()) <= unitWithPlayerControl->GetStatComponent()->GetVitalCurValue(EVitals::Mana))
            {
               if(GetCurrentlySelectedSpell() == spellToCast)
               {
>>>>>>> componentrefactor
                  DeselectSpell();
                  return false;
               }

<<<<<<< HEAD
               if(spell->GetTargeting()->GetTargetTag().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.None"))) {
                  unitOwner->GetUnitController()->Stop();
                  unitOwner->GetUnitController()->FindComponentByClass<USpellCastComponent>()->IncantationCheck(spellToCast);
               } else {
                  CPCRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Magic);
               }

               currentlySelectedSpell = spellToCast;

               if(spell->GetTargeting()->GetTargetTag().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area"))) {
                  // TODO: depending on the spell area targeting, use different indicators
                  CPCRef->GetCameraPawn()->ShowSpellCircle(spell->GetAOE(unitOwner->GetAbilitySystemComponent()));
               } else {
=======
               if(spell->GetTargeting()->IsChildOf(UUpSpellTargeting_None::StaticClass()))
               {
                  unitWithPlayerControl->GetUnitController()->StopCurrentAction();            
                  GetSpellCastComp()->BeginCastSpell(spellToCast);
               }
               else
               {
                  CPCRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Magic);
                  currentlySelectedSpell = spellToCast;
               }

               if(spell->GetTargeting()->IsChildOf(UUpSpellTargeting_Area::StaticClass()))
               {
                  // TODO: depending on the spell area targeting, use different indicators
                  CPCRef->GetCameraPawn()->ShowSpellCircle(spell->GetAOE(GetRTSAbilityComp()));
               }
               else
               {
>>>>>>> componentrefactor
                  CPCRef->GetCameraPawn()->HideSpellCircle();
               }

               return true;
<<<<<<< HEAD
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
=======
            }
            URTSIngameWidget::NativeDisplayHelpText(GetWorld(), NOT_ENOUGH_MANA_TEXT);
            return false;
         } // If spell still not ready
         URTSIngameWidget::NativeDisplayHelpText(GetWorld(), ON_COOLDOWN_TEXT);
         return false;
      }
   }
   currentlySelectedSpell = nullptr;
   return false;
}

void UManualSpellComponent::OnUnitStopped()
{
   currentlySelectedSpell = nullptr;
}

void UManualSpellComponent::OnSkillActivated(int spellIndex)
{
   if(unitWithPlayerControl->GetUnitSelected())
   {
      if(spellIndex >= 0 && spellIndex < GetRTSAbilityComp()->GetAbilities().Num())
      {
         PressedCastSpell(spellIndex);
      }
   }
}

bool UManualSpellComponent::OnSpellConfirmInput(const FHitResult& hitResult, TSubclassOf<UMySpell> selectedSpell)
{
   if(IsValid(selectedSpell) && hitResult.IsValidBlockingHit())
   {
      const UUpSpellTargeting* spellTargeting = selectedSpell.GetDefaultObject()->GetTargeting().GetDefaultObject();
      if(spellTargeting->ManualTargetingCheck(unitWithPlayerControl, hitResult))
      {
         return true;
      }
   }
   URTSIngameWidget::NativeDisplayHelpText(GetWorld(), INVALID_TARGET_TEXT);
   return false;
}

void UManualSpellComponent::StartSpellCastAction(const FHitResult& hitResult, TSubclassOf<UMySpell> selectedSpell)
{
   if(IsValid(selectedSpell))
   {
      currentlySelectedSpell                  = selectedSpell;
      const UUpSpellTargeting* spellTargeting = GetCurrentlySelectedSpell().GetDefaultObject()->GetTargeting().GetDefaultObject();
      spellTargeting->ClickResponse(hitResult, GetCurrentlySelectedSpell(), *this);
   }
}

void UManualSpellComponent::BeginPlay()
{
   Super::BeginPlay();

   GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
      unitWithPlayerControl = Cast<AUnit>(Cast<AUnitController>(GetOwner())->GetUnitOwner());
      unitWithPlayerControl->GetUnitController()->OnUnitStopped().AddUObject(this, &UManualSpellComponent::OnUnitStopped);
      spellCastComp = GetOwner()->FindComponentByClass<USpellCastComponent>();

      if(ensure(spellCastComp))
      {
         spellCastComp->OnSpellCasted().AddUObject(this, &UManualSpellComponent::OnSpellCasted);
      }

      if(const AUserInput* userInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
      {
         userInput->GetLocalPlayer()->GetSubsystem<UGameplayDelegateContext>()->OnSkillActivated().AddUObject(this, &UManualSpellComponent::OnSkillActivated);
      }
   });
}

void UManualSpellComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
   Super::EndPlay(EndPlayReason);
   if(AUserInput* userInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      userInput->GetLocalPlayer()->GetSubsystem<UGameplayDelegateContext>()->OnSkillActivated().RemoveAll(this);
   }
>>>>>>> componentrefactor
}

void UManualSpellComponent::DeselectSpell()
{
<<<<<<< HEAD
   if(AUserInput* cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())) {
      currentlySelectedSpell = nullptr;
      cpcRef->GetCameraPawn()->HideSpellCircle();
      cpcRef->GetCameraPawn()->SetSecondaryCursor(ECursorStateEnum::Select);
=======
   if(AUserInput* cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      currentlySelectedSpell = nullptr;
      cpcRef->GetCameraPawn()->HideSpellCircle();
      cpcRef->GetCameraPawn()->SetSecondaryCursor();
>>>>>>> componentrefactor
   }
}

void UManualSpellComponent::OnSpellCasted(TSubclassOf<UMySpell> spellCasted)
{
<<<<<<< HEAD
   if(AUserInput* cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())) {
      if(cpcRef->GetBasePlayer()->GetFocusedUnit() == GetOwner()) {
         if(const int spellSlotIndex = unitOwner->GetAbilitySystemComponent()->FindSlotIndexOfSpell(spellCasted); spellSlotIndex != INDEX_NONE) {
            SpellHUDEvents::OnFocusedUnitSpellCastedEvent.Broadcast(unitOwner, spellSlotIndex);
=======
   if(AUserInput* cpcRef = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
   {
      if(AUnit* focusedUnit = cpcRef->GetBasePlayer()->GetFocusedUnit())
      {
         if(focusedUnit == unitWithPlayerControl)
         {
            const int spellSlotIndex = GetRTSAbilityComp()->FindSlotIndexOfSpell(spellCasted);
            if(spellSlotIndex != INDEX_NONE)
            {
               SpellGameContext::OnFocusedUnitSpellCastedEvent.Broadcast(unitWithPlayerControl, spellSlotIndex);
            }
>>>>>>> componentrefactor
         }
      }
   }
}

<<<<<<< HEAD
void UManualSpellComponent::OnSpellSlotReplaced(int dropSlotindex, TSubclassOf<UMySpell> spellClass)
{
   unitOwner->GetAbilitySystemComponent()->SetSpellAtSlot(spellClass, dropSlotindex);
=======
URTSAbilitySystemComponent* UManualSpellComponent::GetRTSAbilityComp() const
{
   return unitWithPlayerControl ? unitWithPlayerControl->GetAbilitySystemComponent() : nullptr;
>>>>>>> componentrefactor
}

bool UManualSpellComponent::IsTargetingSelf()
{
<<<<<<< HEAD
   return unitOwner->GetTargetComponent()->GetTargetUnit() == unitOwner;
}

void UManualSpellComponent::FinalizeSpellTargeting(const FUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult)
{
   if(AUserInput* userInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController())) {
      userInput->GetCameraPawn()->HideSpellCircle();
      userInput->GetCameraPawn()->SetSecondaryCursor(); // Just set cursor to to select so the cursor check loop will quickly change the cursor back to normal
      spellTargeting->ManualSetSpellTarget(unitOwner->GetTargetComponent(), hitResult);
      spellCastComp->BeginCastSpell(spellClass);
=======
   return unitWithPlayerControl->GetTargetComponent()->GetTargetUnit() == unitWithPlayerControl;
}

void UManualSpellComponent::FinalizeSpellTargeting(const UUpSpellTargeting* spellTargeting, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult)
{
   if(!USpellDataLibrary::IsStunned(unitWithPlayerControl->GetAbilitySystemComponent()))
   {
      if(AUserInput* userInput = Cast<AUserInput>(GetWorld()->GetFirstPlayerController()))
      {
         userInput->GetCameraPawn()->HideSpellCircle();
         userInput->GetCameraPawn()->SetSecondaryCursor(); // Just set cursor to to select so the cursor check loop will quickly change the cursor back to normal
         spellTargeting->ManualSetSpellTarget(unitWithPlayerControl->GetTargetComponent(), hitResult);
         spellCastComp->BeginCastSpell(spellClass);
         currentlySelectedSpell = nullptr;
      }
>>>>>>> componentrefactor
   }
}
