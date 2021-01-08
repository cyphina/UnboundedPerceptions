#pragma once

#include "SpellTargetingTypes.h"

// TODO: Implement this
struct FUpSpellTargeting_Vector : public FUpSpellTargeting
{
   explicit FUpSpellTargeting_Vector(const FGameplayTag& targetTag) :
      FUpSpellTargeting(targetTag)
   {
   }

   bool ManualTargetingCheck(const FHitResult& hitResult) const override { return false; }

   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override
   {
   }

   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override
   {
   }

   void AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const override
   {
   }

   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override { return false; }

   virtual UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }

   void HandleQueryResult
   (TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
    TSubclassOf<UMySpell>       spellToCast) const override
   {
   }
};
