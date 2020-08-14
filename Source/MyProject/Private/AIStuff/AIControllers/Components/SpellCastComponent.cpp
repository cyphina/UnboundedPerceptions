#include "SpellCastComponent.h"

#include "BrainComponent.h"
#include "TargetComponent.h"

#include "Unit.h"
#include "UnitController.h"

#include "SpellSystem/MySpell.h"
#include "SpellSystem/RTSAbilitySystemComponent.h"

void USpellCastComponent::BeginPlay()
{
   const auto unitControllerOwner = Cast<AUnitController>(GetOwner());
   check(unitControllerOwner);
   unitOwnerRef        = unitControllerOwner->GetUnitOwner();
   abilityComponentRef = unitOwnerRef->GetAbilitySystemComponent();
}

bool USpellCastComponent::CheckSpellCastBreakInvis(TSubclassOf<UMySpell> spellToCast) const
{
   const bool doesSpellBreakInvisibility =
       !spellToCast.GetDefaultObject()->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Combat.Effect.AbilityDoesNotBreakInvisibility"));
   return doesSpellBreakInvisibility;
}

void USpellCastComponent::NotifyAIAboutSpellCast() const
{
   FAIMessage msg(AIMessage_SpellCasted, unitOwnerRef);
   FAIMessage::Send(unitOwnerRef, msg);
}

void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, FVector spellTargetLocation)
{
   unitOwnerRef->GetUnitController()->finishedTurningAction.BindUObject(this, &AUnitController::CastTurnAction);
   unitOwnerRef->state->ChangeState(EUnitState::STATE_CASTING);
   if(unitOwnerRef->GetUnitController()->AdjustPosition(spellClass.GetDefaultObject()->GetRange(abilityComponentRef), spellTargetLocation))
      IncantationCheck(spellClass);
}

void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, AActor* spellTargetActor)
{
   unitOwnerRef->GetUnitController()->finishedTurningAction.BindUObject(this, &AUnitController::CastTurnAction);
   unitOwnerRef->state->ChangeState(EUnitState::STATE_CASTING);
   if(unitOwnerRef->GetUnitController()->AdjustPosition(spellClass.GetDefaultObject()->GetRange(abilityComponentRef), spellTargetActor))
      IncantationCheck(spellClass);
}

bool USpellCastComponent::BeginCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();

   if(IsValid(spell)) {
      if(abilityComponentRef->CanCast(spellToCast)) {
         unitOwnerRef->GetUnitController()->Stop();
         SetCurrentSpell(spellToCast); //Keep track of the spell since we may cast it later (we may have to move into position first)

         if(spell->GetTargetting().GetTagName() == "Skill.Targetting.None") //Non-targetted?  Then just cast it
         {
            IncantationCheck(GetCurrentSpell());
            return true;
         } else { //This spell requires a ground or unit target

            if(spell->GetTargetting().MatchesTag(FGameplayTag::RequestGameplayTag("Skill.Targetting.Area"))) {
               // If the target position is extremely close (like we're targetting the ground underneath us which isn't a problem with click confirm because it would)
               // register the click trace as hitting ourself
               if(FVector::Dist2D(unitOwnerRef->GetTargetLocatiozsdn(), unitOwnerRef->GetActorLocation()) < 5.f) {
                  IncantationCheck(GetCurrentSpell());
                  return true;
               }
               AdjustCastingPosition(spellToCast, unitOwnerRef()->GetTargetLocasdtion());
            } else {
               // If the target unit is ourself
               if(unitOwnerRef->FindComponentByClass<UTargetComponent>()->IsTargettingSelf()) {
                  IncantationCheck(GetCurrentSpell());
                  return true;
               }
               AdjustCastingPosition(spellToCast, GetUnitOwner()->GetTargetUnit());
            }

            return true;
         }
      }
   }
   // If spell cannot be cast or is invalid
   unitOwnerRef->GetUnitController()->Stop();
   return false;
}

bool USpellCastComponent::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
   if(GetOwnerRole() == ROLE_Authority) {
      // Cast the spell and send the controller's brain a message
      if(abilityComponentRef->TryActivateAbilityByClass(spellToCast)) {
         NotifyAIAboutSpellCast();

         onSpellCastedEvent.Broadcast(spellToCast);

         // If this spell requires additional channeling after it is casted, change the state to channeling. Else cast the spell and stop
         if(!spellToCast.Get()->GetDefaultObject<UMySpell>()->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Channeled")))
            unitOwnerRef->GetUnitController()->Stop();
         else {
            unitSpellData.channelTime = spellToCast.GetDefaultObject()->GetSecondaryTime(this);
            unitOwnerRef->state->ChangeState(EUnitState::STATE_CHANNELING);
         }

         if(CheckSpellCastBreakInvis(spellToCast))
            abilityComponentRef->TryRemoveInvisibility();

         return true;
      }
   }
   return false;
}

float USpellCastComponent::GetCurrentChannelTime() const
{
   return unitOwnerRef->state->Channeling.GetCurrentChannelTime();
}

void USpellCastComponent::IncantationCheck(TSubclassOf<UMySpell> spellToCast) const
{
   float castTime = spellToCast.GetDefaultObject()->GetCastTime(ownerRef->GetAbilitySystemComponent());
   if(UNLIKELY(!castTime))
      CastSpell(spellToCast);
   else {
      state->ChangeState(EUnitState::STATE_INCANTATION); //Start channeling
   }
}
