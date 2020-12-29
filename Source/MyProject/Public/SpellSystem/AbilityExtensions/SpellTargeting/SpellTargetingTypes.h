#pragma once
#include "GameplayTagContainer.h"

class USpellCastComponent;
class UTargetComponent;
class IManualTargetingController;
class UMySpell;
class AUnit;
class AInteractableBase;

USTRUCT(NoExport, MinimalAPI)
struct FUpSpellTargeting {
 public:
   explicit FUpSpellTargeting(FGameplayTag targetTag) : targetTag(targetTag) {}
   virtual ~FUpSpellTargeting() = default;

   /** Checks to see if the caster clicked on the right kind of target, and the target is in a valid state */
   virtual bool ManualTargetingCheck(const FHitResult& hitResult) const = 0;
   /** Handles different type of targeting systems involved (single click, two clicks, click on self, etc.) */
   virtual void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const = 0;
   /** Used to target the correct kind of entity (a location, actor, or unit) */
   virtual void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const = 0;
   /** Used to move to the correct position for each target type (to an actor or a location)*/
   virtual void ManualAdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult) const = 0;
   virtual bool ShouldTryAdjustPosition(AUnit* spellCaster) const                                                                                    = 0;
   FGameplayTag GetTargetTag() const { return targetTag; }

 protected:
   FORCEINLINE static AActor* GetHitActor(const FHitResult& hitResult);
   FORCEINLINE static AUnit*  GetHitUnit(const FHitResult& hitResult);
   FORCEINLINE static bool    CheckValidHitActor(const FHitResult& hitResult);

   FORCEINLINE bool IsFriendTargetTag() const;
   FORCEINLINE bool IsEnemyTargetTag() const;
   FORCEINLINE bool IsInteractableTargetTag() const;
   void             ManualTargetSelfOrTarget(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const;

 private:
   FGameplayTag targetTag;
};

USTRUCT(NoExport, MinimalAPI)
struct FUpSpellTargeting_None : public FUpSpellTargeting {
   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override { return false; }
};

USTRUCT(NoExport, MinimalAPI)
struct FUpSpellTargeting_SingleUnit : public FUpSpellTargeting {
   bool ManualTargetingCheck(const FHitResult& hitResult) const override;
   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override;
   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override;
   void ManualAdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult) const override;
   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override;

 private:
   FORCEINLINE bool CheckEligibleSpellTarget(const AUnit* hitUnit) const;
};

USTRUCT(NoExport, MinimalAPI)
struct FUpSpellTargeting_InteractableOrUnit : public FUpSpellTargeting {
   bool ManualTargetingCheck(const FHitResult& hitResult) const override;
   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override;
   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override;
   void ManualAdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult) const override;
   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override

       private : FORCEINLINE bool CheckEligibleSpellTarget(const FHitResult& hitResult) const;
   FORCEINLINE static AInteractableBase* GetHitInteractable(const FHitResult& hitResult);
   FORCEINLINE bool                      IsInteractableAndAllUnitTargetTag() const;
};

USTRUCT(NoExport, MinimalAPI)
struct FUpSpellTargeting_Area : public FUpSpellTargeting {
   bool ManualTargetingCheck(const FHitResult& hitResult) const override;
   void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const override;
   void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const override;
   void ManualAdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, const FHitResult& hitResult) const override;
   bool ShouldTryAdjustPosition(AUnit* spellCaster) const override
};

// TODO: Need to add vector targeting!
//USTRUCT(NoExport, MinimalAPI)
//struct FUpSpellTargeting_Vector : public FUpSpellTargeting {
//};
