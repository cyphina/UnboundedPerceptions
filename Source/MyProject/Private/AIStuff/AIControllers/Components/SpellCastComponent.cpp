#include "AIControllers/Components/SpellCastComponent.h"
#include "BrainComponent.h"
<<<<<<< HEAD
#include "SpellDataLibrary.h"
=======
#include "RTSIngameWidget.h"
#include "SpellDataLibrary.h"
#include "StatEnums.h"
>>>>>>> componentrefactor
#include "AIControllers/Components/RTSStateComponent.h"
#include "SpellSystem/AbilityExtensions/SpellTargeting/SpellTargetingTypes.h"
#include "TargetComponent.h"
#include "UnitMessages.h"
#include "Unit.h"
#include "UnitController.h"
<<<<<<< HEAD
=======
#include "UpStatComponent.h"
>>>>>>> componentrefactor
#include "SpellSystem/MySpell.h"
#include "SpellSystem/RTSAbilitySystemComponent.h"

void USpellCastComponent::BeginPlay()
{
<<<<<<< HEAD
   const auto unitControllerOwner = Cast<AUnitController>(GetOwner());
   check(unitControllerOwner);
   unitOwnerRef = unitControllerOwner->GetUnitOwner();
   unitControllerOwner->OnUnitStopped().AddUObject(this, &USpellCastComponent::OnUnitStopped);
   abilityComponentRef = unitOwnerRef->GetAbilitySystemComponent();
=======
   Super::BeginPlay();
   const auto unitControllerOwner = Cast<AUnitController>(GetOwner());
   check(unitControllerOwner);
   unitControllerOwner->OnUnitStopped().AddUObject(this, &USpellCastComponent::OnUnitStopped);

   GetWorld()->GetTimerManager().SetTimerForNextTick([this, unitControllerOwner]() {
      unitOwnerRef        = Cast<AUnit>(unitControllerOwner->GetPawn());
      abilityComponentRef = unitOwnerRef->GetAbilitySystemComponent();
   });
>>>>>>> componentrefactor
}

bool USpellCastComponent::CheckSpellCastBreakInvis(TSubclassOf<UMySpell> spellToCast) const
{
   const bool doesSpellBreakInvisibility =
       !spellToCast.GetDefaultObject()->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Combat.Effect.AbilityDoesNotBreakInvisibility"));
   return doesSpellBreakInvisibility;
}

void USpellCastComponent::OnUnitStopped()
{
<<<<<<< HEAD
   currentSpell       = nullptr;
   auto& timerManager = GetWorld()->GetTimerManager();
   timerManager.ClearTimer(incantationTimer);
   timerManager.ClearTimer(channelingTimer);
}

void USpellCastComponent::NotifyAIAboutSpellCast() const
{
   const FAIMessage msg(UnitMessages::AIMessage_SpellCasted, unitOwnerRef);
   FAIMessage::Send(unitOwnerRef, msg);
=======
   CancelIncantation();
   if(GetCurrentChannelingTime() > 0)
   {
      unitOwnerRef->GetAbilitySystemComponent()->CancelAbilities();
   }
   CancelChanneling();
   // Remove current spell after
   currentSpell = nullptr;
>>>>>>> componentrefactor
}

int USpellCastComponent::GetRange(TSubclassOf<UMySpell> spellClass) const
{
   return spellClass.GetDefaultObject()->GetRange(abilityComponentRef);
}

void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, FVector spellTargetLocation)
{
<<<<<<< HEAD
   if(const auto stateComp = GetOwner()->FindComponentByClass<URTSStateComponent>(); stateComp) stateComp->ChangeState(EUnitState::STATE_CASTING);
   if(unitOwnerRef->GetUnitController()->AdjustPosition(GetRange(spellClass), spellTargetLocation, [this]() { IncantationCheck(GetCurrentSpell()); })) {
=======
   if(URTSStateComponent* stateComp = GetOwner()->FindComponentByClass<URTSStateComponent>())
   {
      stateComp->ChangeState(EUnitState::STATE_CASTING);
   }

   const float range = static_cast<int>(spellClass.GetDefaultObject()->GetRange(abilityComponentRef));
   if(unitOwnerRef->GetUnitController()->AdjustPosition(range, spellTargetLocation, [this]() {
         IncantationCheck(GetCurrentSpell());
      }))
   {
>>>>>>> componentrefactor
      // We could add some functionality here if we are already in position...
   }
}

void USpellCastComponent::AdjustCastingPosition(TSubclassOf<UMySpell> spellClass, AActor* spellTargetActor)
{
<<<<<<< HEAD
   unitOwnerRef->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_CASTING);
   if(unitOwnerRef->GetUnitController()->AdjustPosition(spellClass.GetDefaultObject()->GetRange(abilityComponentRef), spellTargetActor,
                                                        [this]() { IncantationCheck(GetCurrentSpell()); })) {
      // We could add some functionality here if we are already in position...
   }
=======
   if(URTSStateComponent* stateComp = unitOwnerRef->FindComponentByClass<URTSStateComponent>())
   {
      stateComp->ChangeState(EUnitState::STATE_CASTING);
   }

   const float range = static_cast<int>(spellClass.GetDefaultObject()->GetRange(abilityComponentRef));
   unitOwnerRef->GetUnitController()->AdjustPosition(range, spellTargetActor, [this]() {
      IncantationCheck(GetCurrentSpell());
   });
>>>>>>> componentrefactor
}

bool USpellCastComponent::BeginCastSpell(TSubclassOf<UMySpell> spellToCast)
{
   UMySpell* spell = spellToCast.GetDefaultObject();

   // TODO: Add some checks to AI spellcasting maybe so we can make sure designers don't screw up the set queries?

<<<<<<< HEAD
   if(IsValid(spell)) {
      if(abilityComponentRef->CanCast(spellToCast)) {
         unitOwnerRef->GetUnitController()->Stop();
         currentSpell = spellToCast;

         if(!spell->GetTargeting()->ShouldTryAdjustPosition(unitOwnerRef)) {
            IncantationCheck(GetCurrentSpell());
            return true;
         }

         spell->GetTargeting()->AdjustCastPosition(this, spellToCast, unitOwnerRef->GetTargetComponent());
         return true;
      }
   } else {
      unitOwnerRef->GetUnitController()->Stop();
=======
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
>>>>>>> componentrefactor
   }
   return false;
}

void USpellCastComponent::CastSpell(TSubclassOf<UMySpell> spellToCast)
{
<<<<<<< HEAD
   if(GetOwnerRole() == ROLE_Authority) {
      if(unitOwnerRef->GetAbilitySystemComponent()->TryActivateAbilityByClass(spellToCast)) {
         NotifyAIAboutSpellCast();

=======
   if(GetOwnerRole() == ROLE_Authority)
   {
      if(unitOwnerRef->GetAbilitySystemComponent()->TryActivateAbilityByClass(spellToCast))
      {
>>>>>>> componentrefactor
         OnSpellCasted().Broadcast(spellToCast);

         const float channelTime = spellToCast.GetDefaultObject()->GetSecondaryTime(unitOwnerRef->GetAbilitySystemComponent());

<<<<<<< HEAD
         if(!spellToCast.Get()->GetDefaultObject<UMySpell>()->AbilityTags.HasTag(FGameplayTag::RequestGameplayTag("Skill.Channeled")))
            unitOwnerRef->GetUnitController()->Stop();
         else {
            if(channelTime > 0)
            {
               unitOwnerRef->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_CHANNELING);
               GetWorld()->GetTimerManager().SetTimer(channelingTimer, this, &USpellCastComponent::OnChannelingFinished, channelTime, false, -1.f);
            }
=======
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
>>>>>>> componentrefactor
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
<<<<<<< HEAD
   unitOwnerRef->GetUnitController()->Stop();
=======
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
>>>>>>> componentrefactor
}

void USpellCastComponent::IncantationCheck(TSubclassOf<UMySpell> spellToCast)
{
<<<<<<< HEAD
   const float castTime = spellToCast.GetDefaultObject()->GetCastTime(unitOwnerRef->GetAbilitySystemComponent());
   if(UNLIKELY(castTime <= 0))
      CastSpell(spellToCast);
   else {
      unitOwnerRef->FindComponentByClass<URTSStateComponent>()->ChangeState(EUnitState::STATE_INCANTATION);
=======
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

>>>>>>> componentrefactor
      GetWorld()->GetTimerManager().SetTimer(incantationTimer, FTimerDelegate::CreateUObject(this, &USpellCastComponent::CastSpell, spellToCast), castTime, false, -1.f);
      // TODO: Make a separate animation for this?
      PlayCastAnimIfValid(incantationAnimation, castTime);
   }
}

void USpellCastComponent::CancelIncantation()
{
<<<<<<< HEAD
   if(GetCurrentIncantationTime() > SMALL_NUMBER) {
=======
   if(GetCurrentIncantationTime() > SMALL_NUMBER)
   {
>>>>>>> componentrefactor
      GetWorld()->GetTimerManager().ClearTimer(incantationTimer);
      const FAIMessage msg(UnitMessages::AIMessage_SpellInterrupt, unitOwnerRef);
      FAIMessage::Send(unitOwnerRef->GetUnitController(), msg);
   }
}

void USpellCastComponent::CancelChanneling()
{
   // TODO: Maybe change the message sent for this kind of cancel? It may not be necessary until we further develop the AI system
<<<<<<< HEAD
   if(GetCurrentIncantationTime() > SMALL_NUMBER) {
=======
   if(GetCurrentIncantationTime() > SMALL_NUMBER)
   {
>>>>>>> componentrefactor
      GetWorld()->GetTimerManager().ClearTimer(channelingTimer);
      const FAIMessage msg(UnitMessages::AIMessage_SpellInterrupt, unitOwnerRef);
      FAIMessage::Send(unitOwnerRef->GetUnitController(), msg);
   }
}

float USpellCastComponent::GetCurrentIncantationTime() const
{
<<<<<<< HEAD
   return currentSpell.GetDefaultObject()->GetCastTime(unitOwnerRef->GetAbilitySystemComponent()) - GetWorld()->GetTimerManager().GetTimerRemaining(incantationTimer);
=======
   if(currentSpell)
   {
      if(GetWorld()->GetTimerManager().IsTimerActive(incantationTimer))
      {
         return currentSpell.GetDefaultObject()->GetCastTime(unitOwnerRef->GetAbilitySystemComponent()) -
                GetWorld()->GetTimerManager().GetTimerRemaining(incantationTimer);
      }
   }
   return 0;
>>>>>>> componentrefactor
}

float USpellCastComponent::GetCurrentChannelingTime() const
{
<<<<<<< HEAD
   return currentSpell.GetDefaultObject()->GetSecondaryTime(unitOwnerRef->GetAbilitySystemComponent()) - GetWorld()->GetTimerManager().GetTimerRemaining(channelingTimer);
=======
   if(currentSpell)
   {
      if(GetWorld()->GetTimerManager().IsTimerActive(channelingTimer))
      {
         return currentSpell.GetDefaultObject()->GetSecondaryTime(unitOwnerRef->GetAbilitySystemComponent()) -
                GetWorld()->GetTimerManager().GetTimerRemaining(channelingTimer);
      }
   }
   return 0;
>>>>>>> componentrefactor
}

void USpellCastComponent::PlayCastAnimIfValid(UAnimMontage* montageToPlay, float animPlayTime)
{
#if UE_EDITOR || UE_BUILD_DEVELOPMENT || UE_BUILD_TEST
<<<<<<< HEAD
   if(!castAnimation) {
=======
   if(!castAnimation)
   {
>>>>>>> componentrefactor
      UE_LOG(LogTemp, Error, TEXT("Forgot to add a cast animation lol..."));
      return;
   }
#endif
   unitOwnerRef->PlayAnimMontage(castAnimation, 1 / animPlayTime);
}
