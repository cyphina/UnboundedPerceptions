#include "SingleUnitTargeting.h"

#include "BrainComponent.h"
#include "DA_DefaultTargetingScheme.h"
#include "SpellCastComponent.h"
#include "RTSAbilitySystemComponent.h"
#include "SpellDataLibrary.h"

#include "Unit.h"
#include "ManualTargetingControl.h"
#include "TargetComponent.h"
#include "UnitMessages.h"
#include "EnvironmentQuery/EnvQueryManager.h"

bool UUpSpellTargeting_SingleUnit::CheckEligibleSpellTarget(const AUnit* caster, const AUnit* hitUnit) const
{
   if(caster && hitUnit)
   {
      return true;
   }
   return false;
}

bool UUpSpellTargeting_SingleUnitFriendly::CheckEligibleSpellTarget(const AUnit* caster, const AUnit* hitUnit) const
{
   if(Super::CheckEligibleSpellTarget(caster, hitUnit))
   {
      if(caster->GetIsEnemy() == hitUnit->GetIsEnemy())
      {
         return true;
      }
   }
   return false;
}

bool UUpSpellTargeting_SingleUnitEnemy::CheckEligibleSpellTarget(const AUnit* caster, const AUnit* hitUnit) const
{
   if(Super::CheckEligibleSpellTarget(caster, hitUnit))
   {
      if(caster->GetIsEnemy() != hitUnit->GetIsEnemy())
      {
         return true;
      }
   }
   return false;
}

bool UUpSpellTargeting_SingleUnit::ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const
{
   return CheckValidHitActor(hitResult) && GetHitUnit(hitResult) && USpellDataLibrary::IsAttackable(GetHitUnit(hitResult)->GetAbilitySystemComponent()) &&
          CheckEligibleSpellTarget(caster, GetHitUnit(hitResult));
}

void UUpSpellTargeting_SingleUnit::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
{
   targetComp->SetTarget(GetHitUnit(hitResult));
}

bool UUpSpellTargeting_SingleUnit::IsProperTargetSet(UTargetComponent* targetComp) const
{
   return targetComp->IsTargetingTypeIndex(2);
}

void UUpSpellTargeting_SingleUnit::AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
{
   spellCastComp->AdjustCastingPosition(spellClass, targetComp->GetTargetUnit());
}

bool UUpSpellTargeting_SingleUnit::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !spellCaster->FindComponentByClass<UTargetComponent>()->IsTargetingSelf();
}

void UUpSpellTargeting_SingleUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
{
   sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
}

UEnvQuery* UUpSpellTargeting_SingleUnit::GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const
{
   return targetingSchemes->GetSingleUnitTargetQuery();
}

void UUpSpellTargeting_SingleUnit::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                                     const TSubclassOf<UMySpell> spellToCast) const
{
   if(result.IsValid())
   {
      AUnit* targetResult = Cast<AUnit>(result->GetItemAsActor(0));
      if(targetResult)
      {
         casterRef->GetTargetComponent()->SetTarget(targetResult);
         if(spellCastComponent->BeginCastSpell(spellToCast))
         {
            return;
         }
      }
   }

   const FAIMessage msg(UnitMessages::AIMessage_SpellCastFail, casterRef);
   FAIMessage::Send(casterRef, msg);
}

void UUpSpellTargeting_InteractableOrUnit::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                                             TSubclassOf<UMySpell> spellToCast) const
{
   if(result.IsValid())
   {
      AActor* targetResult = result->GetItemAsActor(0);
      if(targetResult)
      {
         casterRef->GetTargetComponent()->SetTarget(targetResult);
         if(spellCastComponent->BeginCastSpell(spellToCast))
         {
            return;
         }
      }
   }

   const FAIMessage msg(UnitMessages::AIMessage_SpellCastFail, casterRef);
   FAIMessage::Send(casterRef, msg);
}

bool UUpSpellTargeting_InteractableOrUnit::CheckEligibleSpellTarget(const FHitResult& hitResult) const
{
   return GetHitUnit(hitResult) ? true : GetHitInteractable(hitResult) ? true : false;
}

bool UUpSpellTargeting_InteractableOrUnit::ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const
{
   return CheckValidHitActor(hitResult) && CheckEligibleSpellTarget(hitResult) &&
          (!GetHitUnit(hitResult) || USpellDataLibrary::IsAttackable(GetHitUnit(hitResult)->GetAbilitySystemComponent()));
}

void UUpSpellTargeting_InteractableOrUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass,
                                                         IManualTargetingController& sourceUnitController) const
{
   sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
}

void UUpSpellTargeting_InteractableOrUnit::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
{
   GetHitUnit(hitResult) ? targetComp->SetTarget(GetHitUnit(hitResult)) : targetComp->SetTarget(hitResult.Actor.Get());
}

bool UUpSpellTargeting_InteractableOrUnit::IsProperTargetSet(UTargetComponent* targetComp) const
{
   return targetComp->IsTargetingTypeIndex(1);
}

void UUpSpellTargeting_InteractableOrUnit::AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
{
   spellCastComp->AdjustCastingPosition(spellClass, targetComp->GetTargetActorOrUnit());
}

bool UUpSpellTargeting_InteractableOrUnit::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !spellCaster->GetTargetComponent()->IsTargetingSelf();
}

AInteractableBase* UUpSpellTargeting_InteractableOrUnit::GetHitInteractable(const FHitResult& hitResult)
{
   return Cast<AInteractableBase>(GetHitActor(hitResult));
}
