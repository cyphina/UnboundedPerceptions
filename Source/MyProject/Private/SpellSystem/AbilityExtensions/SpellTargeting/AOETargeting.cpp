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

bool UUpSpellTargeting_Area::ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const
{
   return true;
}

void UUpSpellTargeting_Area::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
{
   sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
}

void UUpSpellTargeting_Area::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
{
   GetHitUnit(hitResult) ? targetComp->SetTarget(GetHitUnit(hitResult)) : targetComp->SetTarget(hitResult.Location);
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
      spellCastComp->AdjustCastingPosition(spellClass, targetComp->GetTargetLocation());
   }
}

bool UUpSpellTargeting_Area::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !(FVector::Dist2D(spellCaster->GetTargetComponent()->GetTargetLocationVisit(), spellCaster->GetActorLocation()) < 5.f);
}

UEnvQuery* UUpSpellTargeting_Area::GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* scheme) const
{
   return scheme->GetRadiusTargetQuery();
}

void UUpSpellTargeting_Area::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                               TSubclassOf<UMySpell> spellToCast) const
{
   if(result->IsSuccsessful()) {
      TArray<FVector> locations;
      result->GetAllAsLocations(locations);
      casterRef->GetTargetComponent()->SetTarget(locations[0]);
      if(spellCastComponent->BeginCastSpell(spellToCast)) { return; }
   }

   const FAIMessage msg(UnitMessages::AIMessage_SpellCastFail, casterRef);
   FAIMessage::Send(casterRef, msg);
}
