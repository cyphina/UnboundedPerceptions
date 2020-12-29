#include "AIControllers/Components/SpellCastComponent.h"

#include "BrainComponent.h"
#include "AIControllers/Components/RTSStateComponent.h"
#include "SpellSystem/AbilityExtensions/SpellTargeting/SpellTargetingTypes.h"
#include "TargetComponent.h"

#include "Unit.h"
#include "UnitController.h"

#include "SpellSystem/MySpell.h"
#include "SpellSystem/RTSAbilitySystemComponent.h"

void USpellCastComponent::BeginPlay()
{
   const auto unitControllerOwner = Cast<AUnitController>(GetOwner());
   check(unitControllerOwner);
   unitOwnerRef = unitControllerOwner->GetUnitOwner();
   unitControllerOwner->OnUnitStopped().AddUObject(this, &USpellCastComponent::OnUnitStopped);
   abilityComponentRef = unitOwnerRef->GetAbilitySystemComponent();
}

bool USpellCastComponent::CheckSpellCastBreakInvis(TSubclassOf<UMySpell> spellToCast) const
{
   const bool doesSpellBreakInvisibility =
       !spellToCast.GetDefaultObject()->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Combat.Effect.AbilityDoesNotBreakInvisibility"));
   return doesSpellBreakInvisibility;
}

void USpellCastComponent::OnUnitStopped()
{
   currentSpell       = nullptr;
   auto& timerManager = GetWorld()->GetTimerManager();
   timerManager.ClearTimer(incantationTimer);
   timerManager.ClearTimer(channelingTimer);
}

void USpellCastComponent::NotifyAIAboutSpellCast() const
{
   FAIMessage msg(AIMessage_SpellCasted, unitOwnerRef);
   FAIMessage::Send(unitOwnerRef, msg);
}

int USpellCastComponent::GetRange(TSubclassOf<UMySpell> spellClass) const
{
   return spellClass.GetDefaultObject()->GetRange(abilityComponentRef);
}

void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, FVector spellTargetLocation)
{
   if(const auto stateComp = GetOwner()->FindComponentByClass<URTSStateComponent>(); stateComp) stateComp->ChangeState(EUnitState::STATE_CASTING);
   if(unitOwnerRef->GetUnitController()->AdjustPosition(GetRange(spellClass), spellTargetLocation, [this]() { IncantationCheck(GetCurrentSpell()); })) {
      // We could add some functionality here if we are already in position...
   }
}

void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, AActor* spellTargetActor)
{
   unitOwnerRef->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_CASTING);
   if(unitOwnerRef->GetUnitController()->AdjustPosition(spellClass.GetDefaultObject()->GetRange(abilityComponentRef), spellTargetActor,
                                                        [this]() { IncantationCheck(GetCurrentSpell()); })) {
      // We could add some functionality here if we are already in position...
   }
}

bool USpellCastComponent::SpellRepositionCheckAndFix(TSubclassOf<UMySpell> spellClass)
{
   return unitOwnerRef->GetUnitController()->AdjustPosition(GetRange(spellClass), unitOwnerRef->GetTargetComponent()->GetTargetLocationVisit(),
                                                            [this]() { IncantationCheck(GetCurrentSpell()); });
}

bool USpellCastComponent::BeginCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();

   if(IsValid(spell)) {
      if(abilityComponentRef->CanCast(spellToCast)) {
         unitOwnerRef->GetUnitController()->Stop();
         currentSpell = spellToCast;

         if(!spell->GetTargeting()->ShouldTryAdjustPosition(unitOwnerRef)) {
            IncantationCheck(GetCurrentSpell());
            return true;
         }

         AdjustCastingPosition(spellToCast, unitOwnerRef->GetTargetComponent()->GetTargetLocationVisit());
         return true;
      }
   } else {
      unitOwnerRef->GetUnitController()->Stop();
   }
   return false;
}

void USpellCastComponent::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
   if(GetOwnerRole() == ROLE_Authority) {
      if(unitOwnerRef->GetAbilitySystemComponent()->TryActivateAbilityByClass(spellToCast)) {
         NotifyAIAboutSpellCast();

         OnSpellCasted().Broadcast(spellToCast);

         const float channelTime = spellToCast.GetDefaultObject()->GetSecondaryTime(unitOwnerRef->GetAbilitySystemComponent());

         if(!spellToCast.Get()->GetDefaultObject<UMySpell>()->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Channeled")) && channelTime > 0)
            unitOwnerRef->GetUnitController()->Stop();
         else {
            unitOwnerRef->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_CHANNELING);
            GetWorld()->GetTimerManager().SetTimer(
                incantationTimer,
                [this]() {
                   const FGameplayTag confirmTag = FGameplayTag::RequestGameplayTag("Skill.Confirm");
                   FGameplayEventData eD         = FGameplayEventData();
                   eD.EventTag                   = confirmTag;
                   unitOwnerRef->GetAbilitySystemComponent()->HandleGameplayEvent(confirmTag, &eD);
                },
                channelTime, false, -1.f);
         }

         TestAndPlayAnimation(castAnimation, channelTime);
         if(CheckSpellCastBreakInvis(spellToCast)) abilityComponentRef->TryRemoveInvisibility();
      }
   }
}

void USpellCastComponent::CancelIncantation()
{
   if(GetCurrentIncantationTime() > SMALL_NUMBER) {
      GetWorld()->GetTimerManager().ClearTimer(incantationTimer);
      const FAIMessage msg(UnitMessages::AIMessage_SpellInterrupt, unitOwnerRef);
      FAIMessage::Send(unitOwnerRef->GetUnitController(), msg);
   }
}

void USpellCastComponent::CancelChanneling()
{
   // TODO: Maybe change the message sent for this kind of cancel? It may not be necessary until we further develop the AI system
   if(GetCurrentIncantationTime() > SMALL_NUMBER) {
      GetWorld()->GetTimerManager().ClearTimer(channelingTimer);
      const FAIMessage msg(UnitMessages::AIMessage_SpellInterrupt, unitOwnerRef);
      FAIMessage::Send(unitOwnerRef->GetUnitController(), msg);
   }
}

void USpellCastComponent::IncantationCheck(TSubclassOf<UMySpell> spellToCast)
{
   const float castTime = spellToCast.GetDefaultObject()->GetCastTime(unitOwnerRef->GetAbilitySystemComponent());
   if(UNLIKELY(castTime <= 0))
      CastSpell(spellToCast);
   else {
      unitOwnerRef->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_INCANTATION);
      GetWorld()->GetTimerManager().SetTimer(incantationTimer, FTimerDelegate::CreateUObject(this, &USpellCastComponent::CastSpell, spellToCast), castTime, false, -1.f);
      // TODO: Make a separate animation for i
      TestAndPlayAnimation(incantationAnimation, castTime);
   }
}

float USpellCastComponent::GetCurrentIncantationTime() const
{
   return currentSpell.GetDefaultObject()->GetCastTime(unitOwnerRef->GetAbilitySystemComponent()) - GetWorld()->GetTimerManager().GetTimerRemaining(incantationTimer);
}

float USpellCastComponent::GetCurrentChannelingTime() const
{
   return currentSpell.GetDefaultObject()->GetSecondaryTime(unitOwnerRef->GetAbilitySystemComponent()) - GetWorld()->GetTimerManager().GetTimerRemaining(channelingTimer);
}

void USpellCastComponent::TestAndPlayAnimation(UAnimMontage* montageToPlay, float animPlayTime)
{
#if IF_WITH_EDITOR || UE_BUILD_DEVELOPMENT || UE_BUILD_TEST
   if(!castAnimation) {
      UE_LOG(LogTemp, Error, TEXT("Forgot to add a cast animation lol..."));
      return;
   }
#endif
   unitOwnerRef->PlayAnimMontage(castAnimation, 1 / castTime);
}
