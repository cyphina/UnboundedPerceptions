#pragma once
#include "SpellTargetingTypes.h"

struct FUpSpellTargeting_SingleUnit : public FUpSpellTargeting {
   explicit FUpSpellTargeting_SingleUnit(const FGameplayTag& targetTag) :
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

 private:
   bool CheckEligibleSpellTarget(const AUnit* hitUnit) const;
};

struct FUpSpellTargeting_InteractableOrUnit : public FUpSpellTargeting {
   explicit FUpSpellTargeting_InteractableOrUnit(const FGameplayTag& targetTag) :
      FUpSpellTargeting(targetTag)
   {
   }

   bool ManualTargetingCheck(const FHitResult& hitResult) const override;

   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override;

   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override;

   void AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const override;

   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override;

   virtual UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }

   void HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                          TSubclassOf<UMySpell> spellToCast) const override;

 private:
   bool                      CheckEligibleSpellTarget(const FHitResult& hitResult) const;
   static AInteractableBase* GetHitInteractable(const FHitResult& hitResult);
   bool                      IsInteractableAndAllUnitTargetTag() const;
};
