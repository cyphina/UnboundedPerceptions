#pragma once
#include "SpellTargetingTypes.h"

struct FUpSpellTargeting_Area : public FUpSpellTargeting {
   explicit FUpSpellTargeting_Area(const FGameplayTag& targetTag) :
      FUpSpellTargeting(targetTag)
   {
   }

   bool ManualTargetingCheck(const FHitResult& hitResult) const override;

   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override;

   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override;

   void AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const override;

   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override;

   virtual UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override;

   void HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                          TSubclassOf<UMySpell> spellToCast) const override;
};

