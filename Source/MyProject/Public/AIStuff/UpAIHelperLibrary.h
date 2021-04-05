#pragma once

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpAIHelperLibrary.generated.h"

class USpellCastComponent;
class UMySpell;
class AUnit;
class UEnvQuery;

/**
 * A function library filled with utility to check position of units.
 * Used mainly in movement logic inside controllers to adjust a unit's position before performing an action
 */
UCLASS()
class UUpAIHelperLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_BODY()

 public:
   /**Function for checking if an actor is within a certain distance of some other point
    * @param referenceActor - Actor that we are testing
    * @param range - Distance cutoff for testing
    * @param targetLocation - Point we want to know if our actor is within range
    */
   UFUNCTION(BlueprintCallable, Category = "Positioning")
   static bool IsTargetInRange(const AActor* referenceActor, const FVector& targetLocation, const float range);

   /**Function for checking if an actor is within a certain distance of another actor. Useful since it accounts for the actor bounds when checking for range (i.e., if our target actor
    * is really big then it will check for the distance to the edge of that actor instead of its center)
    * @param referenceActor - Actor that we are testing
    * @param range - Distance cutoff for testing
    * @param targetActor - Actor we want to know if we are in range of.
    */
   UFUNCTION(BlueprintCallable, Category = "Positioning")
   static bool IsTargetInRangeOfActor(const AActor* referenceActor, const AActor* targetActor, const float range);

   /**Function to see if unit is facing towards a point
    * @param referenceActor - Actor that we are testing
    * @param targetLocation - Point we want to see if our actor is facing
    * @param errorAngleCutoff - Angle of the arc that the referenceActor can be within to still be considered facing
    */
   UFUNCTION(BlueprintCallable, Category = "Positioning")
   static bool IsFacingTarget(const AActor* referenceActor, const FVector& targetLocation, const float errorAngleCutoff = .02f);

   UFUNCTION(BlueprintCallable, Category = "Positioning")
   static FQuat FindLookRotation(const AActor* referenceActor, const FVector& targetPoint);

   UFUNCTION(BlueprintCallable, Category = "Positioning")
   static AUnit* FindClosestUnit(const FVector referenceLocation, const TSet<AUnit*>& otherUnits);

   /**
    * @brief Initiates the entire spell casting procedure by triggering the target finding EQS query and passing a callback to it to cast the spell on the target when done.
    * When the procedure is done it notifies the BT that spell casting finished via an AIMessage.
    * @param targetFindingQuery - Used to find target for the spell cast
    * @param spellToCast - Spell we want AI to cast
    * @param spellCastComponent - Spell cast component of the caster
    */
   UFUNCTION(BlueprintCallable, Category = "Spell Casting")
   static void AIBeginCastSpell(UEnvQuery* targetFindingQuery, const TSubclassOf<UMySpell> spellToCast, USpellCastComponent* spellCastComponent,
                                EEnvQueryRunMode::Type queryRunMode = EEnvQueryRunMode::Type::SingleResult);
};
