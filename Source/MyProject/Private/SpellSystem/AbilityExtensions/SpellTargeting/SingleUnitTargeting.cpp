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

bool FUpSpellTargeting_SingleUnit::CheckEligibleSpellTarget(const AUnit* hitUnit) const
{
   if(hitUnit->GetIsEnemy()) {
      return !(IsFriendTargetTag() || IsInteractableTargetTag());
   } else {
      return !(IsEnemyTargetTag() || IsInteractableTargetTag());
   }
}

bool FUpSpellTargeting_SingleUnit::ManualTargetingCheck(const FHitResult& hitResult) const
{
   return CheckValidHitActor(hitResult) && GetHitUnit(hitResult) && USpellDataLibrary::IsAttackable(GetHitUnit(hitResult)->GetAbilitySystemComponent()) &&
          CheckEligibleSpellTarget(GetHitUnit(hitResult));
}

void FUpSpellTargeting_SingleUnit::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
{
   targetComp->SetTarget(GetHitUnit(hitResult));
}

void FUpSpellTargeting_SingleUnit::AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
{
   spellCastComp->AdjustCastingPosition(spellClass, targetComp->GetTargetUnit());
}

bool FUpSpellTargeting_SingleUnit::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !spellCaster->FindComponentByClass<UTargetComponent>()->IsTargetingSelf();
}

void FUpSpellTargeting_SingleUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
{
   sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
}

UEnvQuery* FUpSpellTargeting_SingleUnit::GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const
{
   return targetingSchemes->GetSingleUnitTargetQuery();
}

void FUpSpellTargeting_SingleUnit::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                                     TSubclassOf<UMySpell> spellToCast) const
{
   if(result.IsValid()) {
      casterRef->GetTargetComponent()->SetTarget(Cast<AUnit>(result->GetItemAsActor(0)));
      if(spellCastComponent->BeginCastSpell(spellToCast)) { return; }
   }

   const FAIMessage msg(UnitMessages::AIMessage_SpellCastFail, casterRef);
   FAIMessage::Send(casterRef, msg);
}

void FUpSpellTargeting_InteractableOrUnit::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                                             TSubclassOf<UMySpell> spellToCast) const
{
   if(result.IsValid()) {
      casterRef->GetTargetComponent()->SetTarget(result->GetItemAsActor(0));
      if(spellCastComponent->BeginCastSpell(spellToCast)) { return; }
   }

   const FAIMessage msg(UnitMessages::AIMessage_SpellCastFail, casterRef);
   FAIMessage::Send(casterRef, msg);
}

bool FUpSpellTargeting_InteractableOrUnit::CheckEligibleSpellTarget(const FHitResult& hitResult) const
{
   if(GetHitUnit(hitResult)) return IsInteractableAndAllUnitTargetTag();
   return GetHitInteractable(hitResult) ? true : false;
}

bool FUpSpellTargeting_InteractableOrUnit::ManualTargetingCheck(const FHitResult& hitResult) const
{
   return CheckValidHitActor(hitResult) && CheckEligibleSpellTarget(hitResult) &&
          (!GetHitUnit(hitResult) || USpellDataLibrary::IsAttackable(GetHitUnit(hitResult)->GetAbilitySystemComponent()));
}

void FUpSpellTargeting_InteractableOrUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass,
                                                         IManualTargetingController& sourceUnitController) const
{
   sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
}

void FUpSpellTargeting_InteractableOrUnit::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
{
   GetHitUnit(hitResult) ? targetComp->SetTarget(GetHitUnit(hitResult)) : targetComp->SetTarget(hitResult.Actor.Get());
}

void FUpSpellTargeting_InteractableOrUnit::AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
{
   spellCastComp->AdjustCastingPosition(spellClass, targetComp->GetTargetActorOrUnit());
}

bool FUpSpellTargeting_InteractableOrUnit::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !spellCaster->GetTargetComponent()->IsTargetingSelf();
}

bool FUpSpellTargeting_InteractableOrUnit::IsInteractableAndAllUnitTargetTag() const
{
   return GetTargetTag().GetTagName() == "Skill.Targeting.Single.AllUnitsAndInteractables";
}

AInteractableBase* FUpSpellTargeting_InteractableOrUnit::GetHitInteractable(const FHitResult& hitResult)
{
   return Cast<AInteractableBase>(GetHitActor(hitResult));
}
