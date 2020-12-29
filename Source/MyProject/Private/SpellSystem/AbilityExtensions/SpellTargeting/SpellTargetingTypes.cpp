#include "SpellTargetingTypes.h"
#include "SpellCastComponent.h"
#include "RTSAbilitySystemComponent.h"
#include "SpellDataLibrary.h"

#include "Unit.h"
#include "ManualTargetingControl.h"
#include "TargetComponent.h"

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
   return CheckValidHitActor(hitResult) && GetHitUnit(hitResult) && USpellDataLibrary::IsAttackable(*GetHitUnit(hitResult)->GetAbilitySystemComponent()) &&
          CheckEligibleSpellTarget(GetHitUnit(hitResult));
}

void FUpSpellTargeting_SingleUnit::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
{
   targetComp->SetTarget(GetHitUnit(hitResult));
}

void FUpSpellTargeting_SingleUnit::ManualAdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult) const
{
   spellCastComp->AdjustCastingPosition(spellClass, GetHitActor(hitResult));
}

bool FUpSpellTargeting_SingleUnit::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !spellCaster->FindComponentByClass<UTargetComponent>()->IsTargetingSelf();
}

void FUpSpellTargeting_SingleUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
{
   ManualTargetSelfOrTarget(hitResult, spellClass, sourceUnitController);
}

bool FUpSpellTargeting_InteractableOrUnit::CheckEligibleSpellTarget(const FHitResult& hitResult) const
{
   if(GetHitUnit(hitResult)) return IsInteractableAndAllUnitTargetTag();
   return GetHitInteractable(hitResult) ? true : false;
}

bool FUpSpellTargeting_InteractableOrUnit::ManualTargetingCheck(const FHitResult& hitResult) const
{
   return CheckValidHitActor(hitResult) && CheckEligibleSpellTarget(hitResult) &&
          (!GetHitUnit(hitResult) || USpellDataLibrary::IsAttackable(*GetHitUnit(hitResult)->GetAbilitySystemComponent()));
}

void FUpSpellTargeting_InteractableOrUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass,
                                                         IManualTargetingController& sourceUnitController) const
{
   ManualTargetSelfOrTarget(hitResult, spellClass, sourceUnitController);
}

void FUpSpellTargeting_InteractableOrUnit::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
{
   GetHitUnit(hitResult) ? targetComp->SetTarget(GetHitUnit(hitResult)) : targetComp->SetTarget(hitResult.Actor.Get());
}

void FUpSpellTargeting_InteractableOrUnit::ManualAdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass,
                                                                    const FHitResult& hitResult) const
{
   spellCastComp->AdjustCastingPosition(spellClass, GetHitActor(hitResult));
}

bool FUpSpellTargeting_InteractableOrUnit::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !spellCaster->GetTargetComponent()->IsTargetingSelf();
}

bool FUpSpellTargeting_Area::ManualTargetingCheck(const FHitResult& hitResult) const
{
   return true;
}

void FUpSpellTargeting_Area::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
{
   ManualTargetSelfOrTarget(hitResult, spellClass, sourceUnitController);
}

void FUpSpellTargeting_Area::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
{
   GetHitUnit(hitResult) ? targetComp->SetTarget(GetHitUnit(hitResult)) : targetComp->SetTarget(hitResult.Location);
}

void FUpSpellTargeting_Area::ManualAdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult) const
{
   GetHitUnit(hitResult) ? spellCastComp->AdjustCastingPosition(spellClass, GetHitActor(hitResult))
                         : spellCastComp->AdjustCastingPosition(spellClass, hitResult.Location);
}

bool FUpSpellTargeting_Area::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !FVector::Dist2D(spellCaster->GetTargetComponent()->GetTargetLocation(), spellCaster->GetActorLocation()) < 5.f;
}

AActor* FUpSpellTargeting::GetHitActor(const FHitResult& hitResult)
{
   return hitResult.Actor.Get();
}

AUnit* FUpSpellTargeting::GetHitUnit(const FHitResult& hitResult)
{
   return Cast<AUnit>(GetHitActor(hitResult));
}

AInteractableBase* FUpSpellTargeting_InteractableOrUnit::GetHitInteractable(const FHitResult& hitResult)
{
   return Cast<AInteractableBase>(GetHitActor(hitResult));
}

bool FUpSpellTargeting::CheckValidHitActor(const FHitResult& hitResult)
{
   return IsValid(GetHitActor(hitResult));
}

bool FUpSpellTargeting::IsFriendTargetTag() const
{
   return GetTargetTag().GetTagName() == "Skill.Targeting.Single.Friendly";
}

bool FUpSpellTargeting::IsEnemyTargetTag() const
{
   return GetTargetTag().GetTagName() == "Skill.Targeting.Single.Enemy";
}

bool FUpSpellTargeting::IsInteractableTargetTag() const
{
   return GetTargetTag().GetTagName() == "Skill.Targeting.Single.Interactable";
}

bool FUpSpellTargeting_InteractableOrUnit::IsInteractableAndAllUnitTargetTag() const
{
   return GetTargetTag().GetTagName() == "o	Skill.Targeting.Single.AllUnitsAndInteractables";
}

void FUpSpellTargeting::ManualTargetSelfOrTarget(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
{
   if(sourceUnitController.IsTargetingSelf()) {
      sourceUnitController.CastSpellOnSelf(spellClass);
   } else {
      sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
   }
}
