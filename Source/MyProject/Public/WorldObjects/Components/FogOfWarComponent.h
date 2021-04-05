// Created 12/14/20 11:4 PM

#pragma once

#include "Components/SphereComponent.h"
#include "FogOfWarComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UFogOfWarComponent : public USphereComponent
{
   GENERATED_BODY()

<<<<<<< HEAD
 public:
=======
public:
>>>>>>> componentrefactor
   UFogOfWarComponent();

   /** Sorts the corner points, adds points to make visibility polygon shape approximate a circle,
     * and then orders the trace to find the visibility polygon vertices */
   void FindVisibilityPoints();

   const TArray<FVector>& GetVisionPolygonVertices() const { return visionPolygonVertices; }

<<<<<<< HEAD
 protected:
   void BeginPlay() override;

 private:
=======
protected:
   void BeginPlay() override;

private:
>>>>>>> componentrefactor
   /**
    * @brief When we overlap an obstacle, track the corners do we know how to deform the FOW around it.
    */
   void GetCornersInRange(AActor* overlappedActor);

   /**
    * @brief When we stop overlapping an obstacle, 
    */
   void RemoveCornersInRange(AActor* overlapEndActor);

   /** Used by fog of war plane to figure out how to draw itself, and could be used by AI */
   TArray<FVector> visionPolygonVertices;

   /**
    * @brief Corners of everything in range that can block our vision (walls that have 'VisionBlocker' collision tag)
    */
   TSet<FVector> visionBlockerCorners;

   /**
    * @brief Used to add corners to be taken into account when drawing visibility polygon
    */
   UFUNCTION()
<<<<<<< HEAD
   void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep,
                       const FHitResult& sweepRes);
=======
   void OnBeginOverlap
   (UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep,
    const FHitResult&    sweepRes);
>>>>>>> componentrefactor

   /**
    * @brief used to remove corners of being visibility checked (used to draw visibility polygon)
    */
   UFUNCTION()
   void OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};
