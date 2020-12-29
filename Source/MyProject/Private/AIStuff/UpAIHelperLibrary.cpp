#include "MyProject.h"
#include "UpAIHelperLibrary.h"

#include "Unit.h"

bool UUpAIHelperLibrary::IsTargetInRange(const AActor& referenceActor, const FVector& targetLocation, const float range)
{
   const FVector currentLocation = referenceActor.GetActorLocation();
   FVector       difference      = currentLocation - targetLocation;
   difference.Z                  = 0;

   if(FVector::DotProduct(difference, difference) <= range * range + SMALL_NUMBER) return true;
   return false;
}

bool UUpAIHelperLibrary::IsFacingTarget(const AActor& referenceActor, const FVector& targetLocation, float angleErrorCutoff)
{
   const FVector difference = (targetLocation - referenceActor.GetActorLocation()).GetSafeNormal2D();
   const float   dot        = FVector::DotProduct(referenceActor.GetActorForwardVector(), difference);

   // .05 is 18 degrees lenient by default (only from right side).
   if(dot > 1.f - angleErrorCutoff) return true;
   return false;
}

FQuat UUpAIHelperLibrary::FindLookRotation(const AActor& referenceActor, const FVector& targetPoint)
{
   const FVector difference         = targetPoint - referenceActor.GetActorLocation();
   const FVector projectedDirection = FVector(difference.X, difference.Y, 0);
   return FRotationMatrix::MakeFromX(FVector(projectedDirection)).ToQuat();
}

AUnit* UUpAIHelperLibrary::FindClosestUnit(const AUnit& referenceUnit, const TSet<AUnit*>& otherUnits)
{
   AUnit* closestUnit = nullptr;
   if(otherUnits.Num() > 0) {
      float closestDistance = TNumericLimits<int>::Max();

      for(AUnit* otherUnit : otherUnits) {
         if(const float distToOtherUnit = FVector::Dist2D(referenceUnit.GetActorLocation(), otherUnit->GetActorLocation()); distToOtherUnit < closestDistance) {
            closestDistance = distToOtherUnit;
            closestUnit     = otherUnit;
         }
      }
   }
   return closestUnit;
}
