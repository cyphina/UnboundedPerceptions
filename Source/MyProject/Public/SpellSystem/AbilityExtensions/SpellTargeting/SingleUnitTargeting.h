#pragma once
#include "SpellTargetingTypes.h"
#include "SingleUnitTargeting.generated.h"

/** Allows us to target both friendly and enemy units */
UCLASS()
class UUpSpellTargeting_SingleUnit : public UUpSpellTargeting
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
                          const TSubclassOf<UMySpell> spellToCast) const override;

 protected:
   virtual bool CheckEligibleSpellTarget(const AUnit* caster, const AUnit* hitUnit) const;
};

UCLASS()
class UUpSpellTargeting_SingleUnitFriendly : public UUpSpellTargeting_SingleUnit
{
   GENERATED_BODY()

 private:
   virtual bool CheckEligibleSpellTarget(const AUnit* caster, const AUnit* hitUnit) const override;
};

UCLASS()
class UUpSpellTargeting_SingleUnitEnemy : public UUpSpellTargeting_SingleUnit
{
   GENERATED_BODY()

 private:
   virtual bool CheckEligibleSpellTarget(const AUnit* caster, const AUnit* hitUnit) const override;
};

UCLASS()
class UUpSpellTargeting_InteractableOrUnit : public UUpSpellTargeting
{
   GENERATED_BODY()

 public:
   bool ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const override;

   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override;

   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override;

   bool IsProperTargetSet(UTargetComponent* targetComp) const override;

   void AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const override;

   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override;

   UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }

   void HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                          TSubclassOf<UMySpell> spellToCast) const override;

 private:
   bool                      CheckEligibleSpellTarget(const FHitResult& hitResult) const;
   static AInteractableBase* GetHitInteractable(const FHitResult& hitResult);
};
