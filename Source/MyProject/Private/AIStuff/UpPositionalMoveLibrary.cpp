#include "MyProject.h"
#include "UpPositionalMoveLibrary.h"

bool UUpPositionalMoveLibrary::IsTargetInRange(const AActor& referenceActor, float range, const FVector& targetLocation)
{
   FVector currentLocation = referenceActor.GetActorLocation();
   FVector difference      = currentLocation - targetLocation;
   difference.Z            = 0;

   if(FVector::DotProduct(difference, difference) <= range * range)
      return true;
   return false;
}

bool UUpPositionalMoveLibrary::IsFacingTarget(const AActor& referenceActor, const FVector& targetLocation, float angleErrorCutoff)
{
   FVector difference = (targetLocation - referenceActor.GetActorLocation()).GetSafeNormal2D();
   float   dot        = FVector::DotProduct(referenceActor.GetActorForwardVector(), difference);

   if(dot > 1.f - angleErrorCutoff) // .05 is 18 degrees lenient by default (only from right side).
      return true;
   return false;
}

FQuat UUpPositionalMoveLibrary::FindLookRotation(const AActor& referenceActor, const FVector& targetPoint)
{
   FVector difference         = targetPoint - ownerRef->GetActorLocation();
   FVector projectedDirection = FVector(difference.X, difference.Y, 0);
   return FRotationMatrix::MakeFromX(FVector(projectedDirection)).ToQuat();
}
