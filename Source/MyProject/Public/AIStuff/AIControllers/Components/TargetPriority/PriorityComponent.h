#pragma once

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "UUpPriorityComponent.generated.h"

struct FSpellTargetCriteria;
struct FGameplayTag;
class AUnitController;
class IPriorityCalculation;
class UBehaviorTreeComponent;
class UEnvQueryTest;
class UEnvQuery;
class UMySpell;
struct FEnvQueryResult;

/*
 * This controller component contains logic about how to pick the best targets (in standard scenarios).
 *
 * Sometimes the game gives us tools to use spells outside the box (and may require us to do so like using Retrograde), but
 * the AI isn't smart enough to do that so players will have to improvise.
 *
 * Requires us to have a targeting component on the unit.
 *
 * To control when a spell is actually cast, look at the SpellChoiceComponent.
 * To have per spell custom targeting use a UpSpellPriorityComponent.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUpPriorityComponent : public UActorComponent
{
   GENERATED_BODY()

 public:
   void FindBestTargetForSpell(TSubclassOf<UMySpell> spell);

 protected:
   void BeginPlay() override;

 private:
   AUnitController* unitControllerRef;

   /** The class which groups calculations for a particular type of targeting scheme.
    * Different types of targeting requires AI to generate different target types, thus the
    * calculation pipeline needs to be customized for each targeting scheme.*/
   TUniquePtr<IPriorityCalculation> priorityCalculation;

   /** Holds criteria used to pick targets for different kinds of spells. Depending on the type of spell
    * and the spell's purpose, a different set of tests stored within this object will be picked */
   UPROPERTY(EditAnywhere)
   const FSpellTargetCriteria* targetingCriteria;

   TUniquePtr<IPriorityCalculation> MakePriorityCalculation(FGameplayTag targetingTag) const;

   void OnTargetFound(TSharedPtr<FEnvQueryResult> envQuery);

   FGameplayTag          GetManualTag(TSubclassOf<UMySpell> spell) const;
   FGameplayTagContainer GetDescriptorTags(TSubclassOf<UMySpell> spell) const;

   FORCEINLINE class UTargetComponent* GetTargetComp() const;
   FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComp() const;
   FORCEINLINE void                    StopBehaviorTreeTargetTask() const;
   FORCEINLINE void                    GetCurrentlySelectedSpell() const;
   FORCEINLINE void                    CastCurrentlySelectedSpell() const;
};
