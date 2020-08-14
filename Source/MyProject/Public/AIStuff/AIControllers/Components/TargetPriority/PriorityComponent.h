#pragma once

#include "Components/ActorComponent.h"
#include "UUpPriorityComponent.generated.h"

class AUnitController;
class IPriorityCalculation;
class UBehaviorTreeComponent;
class UEnvQueryTest;
class UEnvQuery;
class UMySpell;
struct FEnvQueryResult;

USTRUCT(BlueprintType, NoExport)
struct FSpellPriorityTest {
   UPROPERTY(EditAnywhere)
   UEnvQueryTest* test;

   UPROPERTY(EditAnywhere)
   float scoreMultiplier;
};

/*
 * This controller component contains logic about how to pick the best targets.
 * Without a custom component, the enemy will probably use very standard algorithms involving picking targets with the highest risk or thread level.
 * With this component, we can add custom functionality.
 * We can mix this up with some randomness too to give the illusion of a chance of survival.
 * Requires us to have a targeting component on the unit.
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
   IPriorityCalculation* priorityCalculation;

   /**Find the best spot for targetting an AOE spell
    *@param isSupport - Find best AOE location to hit the most enemies (false) or friends (true)? */
   virtual void FindBestAOELocation(bool isSupport);

   /**Environment query to find the best place to cast an AOE spell for maximum target hits when healing*/
   UPROPERTY(EditDefaultsOnly)
   TArray<FSpellPriorityTest*> offensiveTests;

   /** Environment query to find the best place to cast an AOE spell for maximum target hits when casting an AOE spell*/
   UPROPERTY(EditDefaultsOnly)
   TArray<FSpellPriorityTest*> supportTests;

   /** Environment query to find best enemy unit to perform an offensive spell.*/
   UPROPERTY(EditDefaultsOnly)
   TArray<FSpellPriorityTest*> purgeTests;

   /** Environment query to find the allied unit that needs healing the most */
   UPROPERTY(EditDefaultsOnly)
   TArray<FSpellPriorityTest*> disableTests;

   /** Environment query to find the enemy that should be stunned */
   UPROPERTY(EditDefaultsOnly)
   TArray<FSpellPriorityTest*> findBestEnemyToStun;

   /** Environment query to find the weakest allied unit.  Allies and enemies have should have different EQueries set*/
   UPROPERTY(EditDefaultsOnly)
   TArray<FSpellPriorityTest*> findBestTargetToBuff;

   FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComp() const;
   FORCEINLINE void                    StopBehaviorTreeTargetTask() const;
   FORCEINLINE void                    GetCurrentlySelectedSpell() const;
   FORCEINLINE void                    CastCurrentlySelectedSpell() const;

   void OnTargetFound(TSharedPtr<FEnvQueryResult> result);
};
