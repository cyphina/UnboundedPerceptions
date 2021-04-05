#include "AIControllers/Components/SpellCastComponent.h"
#include "BrainComponent.h"
#include "RTSIngameWidget.h"
#include "SpellDataLibrary.h"
#include "StatEnums.h"
#include "AIControllers/Components/RTSStateComponent.h"
#include "SpellSystem/AbilityExtensions/SpellTargeting/SpellTargetingTypes.h"
#include "TargetComponent.h"
#include "UnitMessages.h"
#include "Unit.h"
#include "UnitController.h"
#include "UpStatComponent.h"
#include "SpellSystem/MySpell.h"
#include "SpellSystem/RTSAbilitySystemComponent.h"

void USpellCastComponent::BeginPlay()
{
   Super::BeginPlay();
   const auto unitControllerOwner = Cast<AUnitController>(GetOwner());
   check(unitControllerOwner);
   unitControllerOwner->OnUnitStopped().AddUObject(this, &USpellCastComponent::OnUnitStopped);

   GetWorld()->GetTimerManager().SetTimerForNextTick([this, unitControllerOwner]() {
      unitOwnerRef        = Cast<AUnit>(unitControllerOwner->GetPawn());
      abilityComponentRef = unitOwnerRef->GetAbilitySystemComponent();
   });
}

bool USpellCastComponent::CheckSpellCastBreakInvis(TSubclassOf<UMySpell> spellToCast) const
{
   const bool doesSpellBreakInvisibility =
       !spellToCast.GetDefaultObject()->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Combat.Effect.AbilityDoesNotBreakInvisibility"));
   return doesSpellBreakInvisibility;
}

void USpellCastComponent::OnUnitStopped()
{
   CancelIncantation();
   if(GetCurrentChannelingTime() > 0)
   {
      unitOwnerRef->GetAbilitySystemComponent()->CancelAbilities();
   }
   CancelChanneling();
   // Remove current spell after
   currentSpell = nullptr;
}

int USpellCastComponent::GetRange(TSubclassOf<UMySpell> spellClass) const
{
   return spellClass.GetDefaultObject()->GetRange(abilityComponentRef);
}

void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, FVector spellTargetLocation)
{
   if(URTSStateComponent* stateComp = GetOwner()->FindComponentByClass<URTSStateComponent>())
   {
      stateComp->ChangeState(EUnitState::STATE_CASTING);
   }

   const float range = static_cast<int>(spellClass.GetDefaultObject()->GetRange(abilityComponentRef));
   if(unitOwnerRef->GetUnitController()->AdjustPosition(range, spellTargetLocation, [this]() {
         IncantationCheck(GetCurrentSpell());
      }))
   {
      // We could add some functionality here if we are already in position...
   }
}

void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, AActor* spellTargetActor)
{
   if(URTSStateComponent* stateComp = unitOwnerRef->FindComponentByClass<URTSStateComponent>())
   {
      stateComp->ChangeState(EUnitState::STATE_CASTING);
   }

   const float range = static_cast<int>(spellClass.GetDefaultObject()->GetRange(abilityComponentRef));
   unitOwnerRef->GetUnitController()->AdjustPosition(range, spellTargetActor, [this]() {
      IncantationCheck(GetCurrentSpell());
   });
}

bool USpellCastComponent::BeginCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();

   // TODO: Add some checks to AI spellcasting maybe so we can make sure designers don't screw up the set queries?

   if(unitOwnerRef)
   {
      if(IsValid(spell))
      {
         if(CanCast(spellToCast))
         {
            unitOwnerRef->GetUnitController()->StopCurrentAction();
            currentSpell = spellToCast;

            if(!spell->GetTargeting().GetDefaultObject()->ShouldTryAdjustPosition(unitOwnerRef))
            {
               IncantationCheck(GetCurrentSpell());
               return true;
            }

            spell->GetTargeting().GetDefaultObject()->AdjustCastPosition(this, spellToCast, unitOwnerRef->GetTargetComponent());
            return true;
         }
      }
      else
      {
         unitOwnerRef->GetUnitController()->StopCurrentAction();
      }
   }
   return false;
}

void USpellCastComponent::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
   if(GetOwnerRole() == ROLE_Authority)
   {
      if(unitOwnerRef->GetAbilitySystemComponent()->TryActivateAbilityByClass(spellToCast))
      {
         OnSpellCasted().Broadcast(spellToCast);

         const float channelTime = spellToCast.GetDefaultObject()->GetSecondaryTime(unitOwnerRef->GetAbilitySystemComponent());

         if(channelTime <= 0)
         {
            unitOwnerRef->GetUnitController()->FinishCurrentAction();
         }
         else
         {
            if(URTSStateComponent* stateComp = GetOwner()->FindComponentByClass<URTSStateComponent>())
            {
               stateComp->ChangeState(EUnitState::STATE_CHANNELING);
            }
            GetWorld()->GetTimerManager().SetTimer(channelingTimer, this, &USpellCastComponent::OnChannelingFinished, channelTime, false, -1.f);
         }

         PlayCastAnimIfValid(castAnimation, channelTime);
         if(CheckSpellCastBreakInvis(spellToCast)) abilityComponentRef->TryRemoveInvisibility();
      }
   }
}

void USpellCastComponent::OnChannelingFinished()
{
   const FGameplayTag confirmTag = FGameplayTag::RequestGameplayTag("Skill.Confirm");
   FGameplayEventData eD         = FGameplayEventData();
   eD.EventTag                   = confirmTag;
   unitOwnerRef->GetAbilitySystemComponent()->HandleGameplayEvent(confirmTag, &eD);
   unitOwnerRef->GetUnitController()->FinishCurrentAction();
}

bool USpellCastComponent::CanCast(TSubclassOf<UMySpell> spellToCheck) const
{
   UMySpell* spell = spellToCheck.GetDefaultObject();

   if(unitOwnerRef)
   {
      if(UAbilitySystemComponent* ASC = unitOwnerRef->GetAbilitySystemComponent())
      {
         if(IsValid(spell) && ASC->GetActivatableAbilities().ContainsByPredicate([&spellToCheck](const FGameplayAbilitySpec& registeredAbilitySpec) {
               return registeredAbilitySpec.Ability == spellToCheck.GetDefaultObject();
            }))
         {
            if(spell->GetCost(ASC) <= unitOwnerRef->FindComponentByClass<UUpStatComponent>()->GetVitalCurValue(EVitals::Mana))
            {
               if(!spell->IsOnCD(ASC) && !USpellDataLibrary::IsStunned(ASC) && !USpellDataLibrary::IsSilenced(ASC))
               {
                  if(spell->HasSpellSpecificResources(ASC))
                  {
                     return true;
                  }
                  else {
                     URTSIngameWidget::NativeDisplayHelpText(GetWorld(), spell->GetMessageDeficientResources());
                  }
               }
            }
         }
      }
   }
   return false;
}

void USpellCastComponent::IncantationCheck(TSubclassOf<UMySpell> spellToCast)
{
   unitOwnerRef->GetUnitController()->StopMovement();

   const float castTime = spellToCast.GetDefaultObject()->GetCastTime(unitOwnerRef->GetAbilitySystemComponent());
   if(UNLIKELY(castTime <= 0))
   {
      CastSpell(spellToCast);
   }
   else
   {
      if(URTSStateComponent* stateComponent = GetOwner()->FindComponentByClass<URTSStateComponent>())
      {
         stateComponent->ChangeState(EUnitState::STATE_INCANTATION);
      }

      GetWorld()->GetTimerManager().SetTimer(incantationTimer, FTimerDelegate::CreateUObject(this, &USpellCastComponent::CastSpell, spellToCast), castTime, false, -1.f);
      // TODO: Make a separate animation for this?
      PlayCastAnimIfValid(incantationAnimation, castTime);
   }
}

void USpellCastComponent::CancelIncantation()
{
   if(GetCurrentIncantationTime() > SMALL_NUMBER)
   {
      GetWorld()->GetTimerManager().ClearTimer(incantationTimer);
      const FAIMessage msg(UnitMessages::AIMessage_SpellInterrupt, unitOwnerRef);
      FAIMessage::Send(unitOwnerRef->GetUnitController(), msg);
   }
}

void USpellCastComponent::CancelChanneling()
{
   // TODO: Maybe change the message sent for this kind of cancel? It may not be necessary until we further develop the AI system
   if(GetCurrentIncantationTime() > SMALL_NUMBER)
   {
      GetWorld()->GetTimerManager().ClearTimer(channelingTimer);
      const FAIMessage msg(UnitMessages::AIMessage_SpellInterrupt, unitOwnerRef);
      FAIMessage::Send(unitOwnerRef->GetUnitController(), msg);
   }
}

float USpellCastComponent::GetCurrentIncantationTime() const
{
   if(currentSpell)
   {
      if(GetWorld()->GetTimerManager().IsTimerActive(incantationTimer))
      {
         return currentSpell.GetDefaultObject()->GetCastTime(unitOwnerRef->GetAbilitySystemComponent()) -
                GetWorld()->GetTimerManager().GetTimerRemaining(incantationTimer);
      }
   }
   return 0;
}

float USpellCastComponent::GetCurrentChannelingTime() const
{
   if(currentSpell)
   {
      if(GetWorld()->GetTimerManager().IsTimerActive(channelingTimer))
      {
         return currentSpell.GetDefaultObject()->GetSecondaryTime(unitOwnerRef->GetAbilitySystemComponent()) -
                GetWorld()->GetTimerManager().GetTimerRemaining(channelingTimer);
      }
   }
   return 0;
}

void USpellCastComponent::PlayCastAnimIfValid(UAnimMontage* montageToPlay, float animPlayTime)
{
#if UE_EDITOR || UE_BUILD_DEVELOPMENT || UE_BUILD_TEST
   if(!castAnimation)
   {
      UE_LOG(LogTemp, Error, TEXT("Forgot to add a cast animation lol..."));
      return;
   }
#endif
   unitOwnerRef->PlayAnimMontage(castAnimation, 1 / animPlayTime);
}
