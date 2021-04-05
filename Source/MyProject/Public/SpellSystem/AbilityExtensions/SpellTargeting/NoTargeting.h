#pragma once
#include "SpellTargetingTypes.h"
<<<<<<< HEAD

struct FUpSpellTargeting_None : public FUpSpellTargeting {
   explicit FUpSpellTargeting_None(const FGameplayTag& targetTag) : FUpSpellTargeting(targetTag) {}
   
   bool ManualTargetingCheck(const FHitResult& hitResult) const override { return true; }
=======
#include "NoTargeting.generated.h"

UCLASS()
class UUpSpellTargeting_None : public UUpSpellTargeting
{
   GENERATED_BODY()

 public:
   bool ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const override { return true; }
>>>>>>> componentrefactor

   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override {}

   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override {}

   void AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const override {}

<<<<<<< HEAD
   virtual UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }
=======
   virtual bool IsProperTargetSet(UTargetComponent* targetComp) const override { return true; }

   UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }
>>>>>>> componentrefactor

   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override { return false; }

   void HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent, TSubclassOf<UMySpell> spellToCast) const override
   {
      check(false);
   }
};
