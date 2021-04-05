#pragma once

#include "SpellTargetingTypes.h"
<<<<<<< HEAD

// TODO: Implement this
struct FUpSpellTargeting_Vector : public FUpSpellTargeting
{
   explicit FUpSpellTargeting_Vector(const FGameplayTag& targetTag) :
      FUpSpellTargeting(targetTag)
   {
   }

   bool ManualTargetingCheck(const FHitResult& hitResult) const override { return false; }
=======
#include "VectorTargeting.generated.h"

// TODO: Implement this
UCLASS()
class UUpSpellTargeting_Vector : public UUpSpellTargeting
{
   GENERATED_BODY()

public:
   bool ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const override { return false; }
>>>>>>> componentrefactor

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

<<<<<<< HEAD
   virtual UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }
=======
   UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }
>>>>>>> componentrefactor

   void HandleQueryResult
   (TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
    TSubclassOf<UMySpell>       spellToCast) const override
   {
   }
};
