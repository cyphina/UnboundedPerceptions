#pragma once
#include "SpellTargetingTypes.h"
<<<<<<< HEAD

struct FUpSpellTargeting_SingleUnit : public FUpSpellTargeting {
   explicit FUpSpellTargeting_SingleUnit(const FGameplayTag& targetTag) :
      FUpSpellTargeting(targetTag)
   {
   }

   bool ManualTargetingCheck(const FHitResult& hitResult) const override;
=======
#include "SingleUnitTargeting.generated.h"

/** Allows us to target both friendly and enemy units */
UCLASS()
class UUpSpellTargeting_SingleUnit : public UUpSpellTargeting
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
=======
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
   virtual UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }
=======
   UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const override { return nullptr; }
>>>>>>> componentrefactor

   void HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
                          TSubclassOf<UMySpell> spellToCast) const override;

 private:
   bool                      CheckEligibleSpellTarget(const FHitResult& hitResult) const;
   static AInteractableBase* GetHitInteractable(const FHitResult& hitResult);
<<<<<<< HEAD
   bool                      IsInteractableAndAllUnitTargetTag() const;
=======
>>>>>>> componentrefactor
};
