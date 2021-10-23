#pragma once
#include "SpellTargetingTypes.h"
#include "AOETargeting.generated.h"

UCLASS()
class UUpSpellTargeting_Area : public UUpSpellTargeting
{
   GENERATED_BODY()
 public:
   bool ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const override;

   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override;

   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override;

   bool IsProperTargetSet(UTargetComponent* targetComp) const override;

   void AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const override;

   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override;

   UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override;

   void HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                          TSubclassOf<UMySpell> spellToCast) const override;
};
