#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpAIHelperLibrary.generated.h"

class AUnit;

/**
 * A function library filled with utility to check position of units.
 * Used mainly in movement logic inside controllers to adjust a unit's position before performing an action
 */
UCLASS()
class UUpAIHelperLibrary : public UBlueprintFunctionLibrary
{
 public:
   /**Function for checking if an actor is within a certain distance of some other point
    * @param referenceActor - Actor that we are testing
    * @param range - Distance cutoff for testing
    * @param targetLocation - Point we want to know if our actor is within range
    */
   UFUNCTION(BlueprintCallable, Category = "Positioning")
   static bool IsTargetInRange(const AActor& referenceActor, const FVector& targetLocation, const float range);

   /**Function to see if unit is facing towards a point
    * @param referenceActor - Actor that we are testing
    * @param targetLocation - Point we want to see if our actor is facing
    * @param errorAngleCutoff - Angle of the arc that the referenceActor can be within to still be considered facing
    */
   UFUNCTION(BlueprintCallable, Category = "Positioning")
   static bool IsFacingTarget(const AActor& referenceActor, const FVector& targetLocation, const float errorAngleCutoff = .02f);

   UFUNCTION(BlueprintCallable, Category = "Positioning")
   static FQuat FindLookRotation(const AActor& referenceActor, const FVector& targetPoint);

   UFUNCTION(BlueprintCallable, Category = "Positioning")
   static AUnit* FindClosestUnit(const AUnit& referenceUnit, const TSet<AUnit*>& otherUnits);
};
