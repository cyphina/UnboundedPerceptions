#pragma once

#include "SpellTargetingTypes.h"
#include "VectorTargeting.generated.h"

// TODO: Implement this
UCLASS()
class UUpSpellTargeting_Vector : public UUpSpellTargeting
{
   GENERATED_BODY()

public:
   bool ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const override { return false; }

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

   UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }

   void HandleQueryResult
   (TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
    TSubclassOf<UMySpell>       spellToCast) const override
   {
   }
};
