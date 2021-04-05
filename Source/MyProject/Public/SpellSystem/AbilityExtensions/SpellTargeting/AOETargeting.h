#pragma once
#include "SpellTargetingTypes.h"
<<<<<<< HEAD

struct FUpSpellTargeting_Area : public FUpSpellTargeting {
   explicit FUpSpellTargeting_Area(const FGameplayTag& targetTag) :
      FUpSpellTargeting(targetTag)
   {
   }

   bool ManualTargetingCheck(const FHitResult& hitResult) const override;
=======
#include "AOETargeting.generated.h"

UCLASS()
class UUpSpellTargeting_Area : public UUpSpellTargeting
{
   GENERATED_BODY()
 public:
   bool ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const override;
>>>>>>> componentrefactor

   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override;

   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override;

<<<<<<< HEAD
=======
   bool IsProperTargetSet(UTargetComponent* targetComp) const override;

>>>>>>> componentrefactor
   void AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const override;

   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override;

<<<<<<< HEAD
   virtual UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override;
=======
   UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override;
>>>>>>> componentrefactor

   void HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                          TSubclassOf<UMySpell> spellToCast) const override;
};
<<<<<<< HEAD

=======
>>>>>>> componentrefactor
