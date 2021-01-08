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

bool FUpSpellTargeting_Area::ManualTargetingCheck(const FHitResult& hitResult) const
{
   return true;
}

void FUpSpellTargeting_Area::ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
{
   sourceUnitController.FinalizeSpellTargeting(this, spellClass, hitResult);
}

void FUpSpellTargeting_Area::ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const
{
   GetHitUnit(hitResult) ? targetComp->SetTarget(GetHitUnit(hitResult)) : targetComp->SetTarget(hitResult.Location);
}

void FUpSpellTargeting_Area::AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
{
   if(AActor* targetActor = targetComp->GetTargetUnit()) {
      spellCastComp->AdjustCastingPosition(spellClass, targetActor);
   } else {
      spellCastComp->AdjustCastingPosition(spellClass, targetComp->GetTargetLocation());
   }
}

bool FUpSpellTargeting_Area::ShouldTryAdjustPosition(AUnit* spellCaster) const
{
   return !(FVector::Dist2D(spellCaster->GetTargetComponent()->GetTargetLocation(), spellCaster->GetActorLocation()) < 5.f);
}

UEnvQuery* FUpSpellTargeting_Area::GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* scheme) const
{
   return scheme->GetRadiusTargetQuery();
}

void FUpSpellTargeting_Area::HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                                               TSubclassOf<UMySpell> spellToCast) const
{
   if(result->IsSuccsessful()) {
      casterRef->GetTargetComponent()->SetTarget(result->GetItemAsLocation(0));
      if(spellCastComponent->BeginCastSpell(spellToCast)) { return; }
   }

   const FAIMessage msg(UnitMessages::AIMessage_SpellCastFail, casterRef);
   FAIMessage::Send(casterRef, msg);
}
