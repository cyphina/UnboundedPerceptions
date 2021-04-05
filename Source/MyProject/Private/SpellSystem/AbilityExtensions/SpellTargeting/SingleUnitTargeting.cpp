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

<<<<<<< HEAD
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
=======
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
>>>>>>> componentrefactor
{
   targetComp->SetTarget(GetHitUnit(hitResult));
}

<<<<<<< HEAD
void FUpSpellTargeting_SingleUnit::AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
=======
bool UUpSpellTargeting_SingleUnit::IsProperTargetSet(UTargetComponent* targetComp) const
{
   return targetComp->IsTargetingTypeIndex(2);
}

void UUpSpellTargeting_SingleUnit::AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
>>>>>>> componentrefactor
{
   spellCastComp->AdjustCastingPosition(spellClass, targetComp->GetTargetUnit());
}

<<<<<<< HEAD
bool FUpSpellTargeting_SingleUnit::ShouldTryAdjustPosition(AUnit* spellCaster) const
=======
bool UUpSpellTargeting_SingleUnit::ShouldTryAdjustPosition(AUnit* spellCaster) const
>>>>>>> componentrefactor
{
   return !spellCaster->FindComponentByClass<UTargetComponent>()->IsTargetingSelf();
}

<<<<<<< HEAD
void FUpSpellTargeting_SingleUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
=======
void UUpSpellTargeting_SingleUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
>>>>>>> componentrefactor
{
   sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
}

<<<<<<< HEAD
UEnvQuery* FUpSpellTargeting_SingleUnit::GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const
=======
UEnvQuery* UUpSpellTargeting_SingleUnit::GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const
>>>>>>> componentrefactor
{
   return targetingSchemes->GetSingleUnitTargetQuery();
}

<<<<<<< HEAD
void FUpSpellTargeting_SingleUnit::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                                     TSubclassOf<UMySpell> spellToCast) const
{
   if(result.IsValid()) {
      casterRef->GetTargetComponent()->SetTarget(Cast<AUnit>(result->GetItemAsActor(0)));
      if(spellCastComponent->BeginCastSpell(spellToCast)) { return; }
=======
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
>>>>>>> componentrefactor
   }

   const FAIMessage msg(UnitMessages::AIMessage_SpellCastFail, casterRef);
   FAIMessage::Send(casterRef, msg);
}

<<<<<<< HEAD
void FUpSpellTargeting_InteractableOrUnit::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                                             TSubclassOf<UMySpell> spellToCast) const
{
   if(result.IsValid()) {
      casterRef->GetTargetComponent()->SetTarget(result->GetItemAsActor(0));
      if(spellCastComponent->BeginCastSpell(spellToCast)) { return; }
=======
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
>>>>>>> componentrefactor
   }

   const FAIMessage msg(UnitMessages::AIMessage_SpellCastFail, casterRef);
   FAIMessage::Send(casterRef, msg);
}

<<<<<<< HEAD
bool FUpSpellTargeting_InteractableOrUnit::CheckEligibleSpellTarget(const FHitResult& hitResult) const
{
   if(GetHitUnit(hitResult)) return IsInteractableAndAllUnitTargetTag();
   return GetHitInteractable(hitResult) ? true : false;
}

bool FUpSpellTargeting_InteractableOrUnit::ManualTargetingCheck(const FHitResult& hitResult) const
=======
bool UUpSpellTargeting_InteractableOrUnit::CheckEligibleSpellTarget(const FHitResult& hitResult) const
{
   return GetHitUnit(hitResult) ? true : GetHitInteractable(hitResult) ? true : false;
}

bool UUpSpellTargeting_InteractableOrUnit::ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const
>>>>>>> componentrefactor
{
   return CheckValidHitActor(hitResult) && CheckEligibleSpellTarget(hitResult) &&
          (!GetHitUnit(hitResult) || USpellDataLibrary::IsAttackable(GetHitUnit(hitResult)->GetAbilitySystemComponent()));
}

<<<<<<< HEAD
void FUpSpellTargeting_InteractableOrUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass,
=======
void UUpSpellTargeting_InteractableOrUnit::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass,
>>>>>>> componentrefactor
                                                         IManualTargetingController& sourceUnitController) const
{
   sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
}

<<<<<<< HEAD
void FUpSpellTargeting_InteractableOrUnit::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
=======
void UUpSpellTargeting_InteractableOrUnit::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
>>>>>>> componentrefactor
{
   GetHitUnit(hitResult) ? targetComp->SetTarget(GetHitUnit(hitResult)) : targetComp->SetTarget(hitResult.Actor.Get());
}

<<<<<<< HEAD
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
=======
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
>>>>>>> componentrefactor
{
   return Cast<AInteractableBase>(GetHitActor(hitResult));
}
