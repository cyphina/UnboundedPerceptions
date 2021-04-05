#pragma once
#include "GameplayTagContainer.h"
<<<<<<< HEAD
=======
#include "Object.h"
#include "SpellTargetingTypes.generated.h"
>>>>>>> componentrefactor

struct FEnvQueryResult;
class UDA_DefaultTargetingScheme;
class UEnvQuery;
class USpellCastComponent;
class UTargetComponent;
class IManualTargetingController;
class UMySpell;
class AUnit;
class AInteractableBase;

<<<<<<< HEAD
struct FUpSpellTargeting
{
public:
   explicit FUpSpellTargeting(FGameplayTag targetTag) :
      targetTag(targetTag)
   {
   }

   virtual ~FUpSpellTargeting() = default;

   /** Checks to see if the caster clicked on the right kind of target, and the target is in a valid state */
   virtual bool ManualTargetingCheck(const FHitResult& hitResult) const = 0;

   /** Handles different type of targeting systems involved (single click, two clicks, click on self, etc.) */
   virtual void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const = 0;

   /** Used to target the correct kind of entity (a location, actor, or unit) */
   virtual void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const = 0;

   /** Used to move to the correct position for each target type (to an actor or a location)*/
   virtual void AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const = 0;

   virtual bool ShouldTryAdjustPosition(AUnit* spellCaster) const = 0;

   virtual UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const = 0;

   virtual void HandleQueryResult
   (TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent,
    TSubclassOf<UMySpell>       spellToCast) const = 0;

   FGameplayTag GetTargetTag() const { return targetTag; }

protected:
   static AActor* GetHitActor(const FHitResult& hitResult);
   static AUnit*  GetHitUnit(const FHitResult& hitResult);
   static bool    CheckValidHitActor(const FHitResult& hitResult);

   bool IsFriendTargetTag() const;
   bool IsEnemyTargetTag() const;
   bool IsInteractableTargetTag() const;

private:
   FGameplayTag targetTag;
};

// TODO: Need to add vector targeting!
//USTRUCT(NoExport, MinimalAPI)
//struct FUpSpellTargeting_Vector : public FUpSpellTargeting {
//};
=======
UCLASS(Abstract)
class UUpSpellTargeting : public UObject
{
   GENERATED_BODY()

 public:
   /** Checks to see if the caster clicked on the right kind of target, and the target is in a valid state */
   virtual bool ManualTargetingCheck(const AUnit* caster, const FHitResult& hitResult) const PURE_VIRTUAL(UUpSpellTargeting::ManualTargetingCheck, return false;);

   /** Handles different type of targeting systems involved (single click, two clicks, click on self, etc.) */
   virtual void ClickResponse(const FHitResult& hitResult, TSubclassOf<UMySpell> spellClass, IManualTargetingController& sourceUnitController) const
       PURE_VIRTUAL(UUpSpellTargeting::ClickResponse, );

   /** Used to target the correct kind of entity (a location, actor, or unit) */
   virtual bool IsProperTargetSet(UTargetComponent* targetComp) const PURE_VIRTUAL(UUpSpellTargeting::IsProperTargetSet, return false; );

   virtual void ManualSetSpellTarget(UTargetComponent* targetComp, const FHitResult& hitResult) const PURE_VIRTUAL(UUpSpellTargeting::ManualSetSpellTarget, );

   /** Used to move to the correct position for each target type (to an actor or a location)*/
   virtual void AdjustCastPosition(USpellCastComponent* spellCastComp, TSubclassOf<UMySpell> spellClass, UTargetComponent* targetComp) const
       PURE_VIRTUAL(UUpSpellTargeting::AdjustCastPosition, );

   virtual bool ShouldTryAdjustPosition(AUnit* spellCaster) const PURE_VIRTUAL(UUpSpellTargeting::ShouldTryAdjustPosition, return false;);

   virtual UEnvQuery* GetDefaultQueryForTargetingScheme(UDA_DefaultTargetingScheme* targetingSchemes) const
       PURE_VIRTUAL(UUpSpellTargeting::GetDefaultQueryForTargetingScheme, return nullptr;);

   virtual void HandleQueryResult(TSharedPtr<FEnvQueryResult> result, AUnit* casterRef, USpellCastComponent* spellCastComponent, TSubclassOf<UMySpell> spellToCast) const
       PURE_VIRTUAL(UUpSpellTargeting::HandleQueryResult, );

 protected:
   static AActor* GetHitActor(const FHitResult& hitResult);
   static AUnit*  GetHitUnit(const FHitResult& hitResult);
   static bool    CheckValidHitActor(const FHitResult& hitResult);
};
>>>>>>> componentrefactor
