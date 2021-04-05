#include "AOETargeting.h"

#include "BrainComponent.h"
#include "DA_DefaultTargetingScheme.h"
#include "SpellCastComponent.h"
#include "RTSAbilitySystemComponent.h"

#include "Unit.h"
#include "ManualTargetingControl.h"
#include "TargetComponent.h"
#include "UnitMessages.h"
#include "EnvironmentQuery/EnvQueryManager.h"

<<<<<<< HEAD
bool FUpSpellTargeting_Area::ManualTargetingCheck(const FHitResult& hitResult) const
=======
bool UUpSpellTargeting_Area::ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const
>>>>>>> componentrefactor
{
   return true;
}

<<<<<<< HEAD
void FUpSpellTargeting_Area::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
=======
void UUpSpellTargeting_Area::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
>>>>>>> componentrefactor
{
   sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
}

<<<<<<< HEAD
void FUpSpellTargeting_Area::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
=======
void UUpSpellTargeting_Area::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
>>>>>>> componentrefactor
{
   GetHitUnit(hitResult) ? targetComp->SetTarget(GetHitUnit(hitResult)) : targetComp->SetTarget(hitResult.Location);
}

<<<<<<< HEAD
void FUpSpellTargeting_Area::AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
{
   if(AActor* targetActor = targetComp->GetTargetUnit()) {
      spellCastComp->AdjustCastingPosition(spellClass, targetActor);
   } else {
=======
bool UUpSpellTargeting_Area::IsProperTargetSet(UTargetComponent* targetComp) const
{
   return !targetComp->IsTargetingTypeIndex(0);
}

void UUpSpellTargeting_Area::AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
{
   if(targetComp->IsTargetingUnit())
   {
      if(AActor* targetActor = targetComp->GetTargetUnit())
      {
         spellCastComp->AdjustCastingPosition(spellClass, targetActor);
      }
   }
   else
   {
>>>>>>> componentrefactor
      spellCastComp->AdjustCastingPosition(spellClass, targetComp->GetTargetLocation());
   }
}

<<<<<<< HEAD
bool FUpSpellTargeting_Area::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !(FVector::Dist2D(spellCaster->GetTargetComponent()->GetTargetLocation(), spellCaster->GetActorLocation()) < 5.f);
}

UEnvQuery* FUpSpellTargeting_Area::GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* scheme) const
=======
bool UUpSpellTargeting_Area::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !(FVector::Dist2D(spellCaster->GetTargetComponent()->GetTargetLocationVisit(), spellCaster->GetActorLocation()) < 5.f);
}

UEnvQuery* UUpSpellTargeting_Area::GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* scheme) const
>>>>>>> componentrefactor
{
   return scheme->GetRadiusTargetQuery();
}

<<<<<<< HEAD
void FUpSpellTargeting_Area::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                               TSubclassOf<UMySpell> spellToCast) const
{
   if(result->IsSuccsessful()) {
      casterRef->GetTargetComponent()->SetTarget(result->GetItemAsLocation(0));
      if(spellCastComponent->BeginCastSpell(spellToCast)) { return; }
=======
void UUpSpellTargeting_Area::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                               TSubclassOf<UMySpell> spellToCast) const
{
   if(result->IsSuccsessful())
   {
      TArray<FVector> locations;
      result->GetAllAsLocations(locations);
      casterRef->GetTargetComponent()->SetTarget(locations[0]);
      if(spellCastComponent->BeginCastSpell(spellToCast))
      {
         return;
      }
>>>>>>> componentrefactor
   }

   const FAIMessage msg(UnitMessages::AIMessage_SpellCastFail, casterRef);
   FAIMessage::Send(casterRef, msg);
}
