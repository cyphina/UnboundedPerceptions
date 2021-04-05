// Created 12/14/20 11:5 PM

#include "MyProject.h"
#include "FogOfWarComponent.h"
#include "UpResourceManager.h"

DECLARE_CYCLE_STAT(TEXT("FOW Polygon Calculation"), STAT_VisionPoly, STATGROUP_RTSUnits)

UFogOfWarComponent::UFogOfWarComponent()
{
 
}

void UFogOfWarComponent::BeginPlay()
{
   OnComponentBeginOverlap.AddDynamic(this, &UFogOfWarComponent::OnBeginOverlap);
   OnComponentEndOverlap.AddDynamic(this, &UFogOfWarComponent::OnEndOverlap);
}

void UFogOfWarComponent::GetCornersInRange(AActor* overlapActor)
{
   FVector origin = FVector();
   FVector extent = FVector();
   overlapActor->GetActorBounds(true, origin, extent);

   // Add corners of the vision blocker (scenery object we overlapped)
   for(FVector v : UpResourceManager::BoundsPointMapping2D) {
      FVector corner     = origin + extent * v;
      FVector cornerDist = corner - GetOwner()->GetActorLocation();
      visionBlockerCorners.Add(corner);
   }
}

void UFogOfWarComponent::RemoveCornersInRange(AActor* overlapEndActor)
{
   FVector origin = FVector();
   FVector extent = FVector();
   overlapEndActor->GetActorBounds(true, origin, extent);

   // Remove corners of the vision blocker (scenery object we overlapped)
   for(FVector v : UpResourceManager::BoundsPointMapping2D) {
      FVector corner     = origin + extent * v;
      FVector cornerDist = corner - GetOwner()->GetActorLocation();
      visionBlockerCorners.Remove(corner);
   }
}

void UFogOfWarComponent::FindVisibilityPoints()
{
   SCOPE_CYCLE_COUNTER(STAT_VisionPoly)
   {
      static auto pred = [this](const FVector& a, const FVector& b) {
         return UpResourceManager::FindOrientation(a - GetOwner()->GetActorLocation()) >
                UpResourceManager::FindOrientation(b - GetOwner()->GetOwner()->GetActorLocation());
      };

      static const float root2Over2 = .70710f;

      visionPolygonVertices.Empty();
      visionPolygonVertices.Reserve(visionBlockerCorners.Num() + 8); // 8 extra points plus corners from visibility blockers

      // Add extra points to make resulting shape close to a circle
      const float visionRadius = GetUnscaledSphereRadius();

      TSet<FVector, DefaultKeyFuncs<FVector>, TInlineSetAllocator<8>> calculationCorners = TSet<FVector>(visionBlockerCorners);
      calculationCorners.Add(GetOwner()->GetActorLocation() + FVector(0, visionRadius, 0));
      calculationCorners.Add(GetOwner()->GetActorLocation() + FVector(visionRadius * root2Over2, visionRadius * root2Over2, 0));
      calculationCorners.Add(GetOwner()->GetActorLocation() + FVector(visionRadius, 0, 0));
      calculationCorners.Add(GetOwner()->GetActorLocation() + FVector(visionRadius * root2Over2, visionRadius * -root2Over2, 0));
      calculationCorners.Add(GetOwner()->GetActorLocation() + FVector(0, -visionRadius, 0));
      calculationCorners.Add(GetOwner()->GetActorLocation() + FVector(visionRadius * -root2Over2, visionRadius * -root2Over2, 0));
      calculationCorners.Add(GetOwner()->GetActorLocation() + FVector(-visionRadius, 0, 0));
      calculationCorners.Add(GetOwner()->GetActorLocation() + FVector(visionRadius * -root2Over2, visionRadius * root2Over2, 0));

      // Sort the points by angle (so they are arranged in a circle since we have corner points from the vision blockers sporadically placed in the set)
      // This allows us to easily create triangles to fill out the "circle".
      calculationCorners.Sort(pred);
      visionPolygonVertices.Add(GetOwner()->GetActorLocation());

      // Find the points of our visibility polygon by tracing and adding any hit points or else the bound points
      for(FVector& c : calculationCorners) {
         FVector dist = c - GetOwner()->GetActorLocation();
         // Filter corner points from visibility blockers (since maybe only one corner is in vision radius) that are outside our vision radius. Possibly not needed?
         if(dist.SizeSquared2D() < visionRadius * visionRadius + 1) {
            FHitResult r;
            if(GetWorld()->LineTraceSingleByChannel(r, GetOwner()->GetActorLocation(), c, UNIT_VISION_CHANNEL)) {
               // DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), r.ImpactPoint, FColor::Red, false, 5, 0, 10);
               visionPolygonVertices.Add(r.ImpactPoint);
            } else {
               // DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), c, FColor::Red, false, 5, 0, 10);
               visionPolygonVertices.Add(c);
            }
         }
      }
   }
}

void UFogOfWarComponent::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex,
                                        bool fromSweep, const FHitResult& sweepRes)
{
   switch(otherActor->GetRootComponent()->GetCollisionObjectType()) {
      case VISION_BLOCKER_CHANNEL: {
         GetCornersInRange(otherActor);
         break;
      }
      default: break;
   }
}

void UFogOfWarComponent::OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
   switch(otherActor->GetRootComponent()->GetCollisionObjectType()) {
      // Remove corners of our vision blocker if we get out of range
      case VISION_BLOCKER_CHANNEL: {
         RemoveCornersInRange(otherActor);
         break;
      }
      default: break;
   }
}
