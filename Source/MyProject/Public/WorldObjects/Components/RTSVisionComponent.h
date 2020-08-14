#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "RTSVisionComponent.generated.h"

class AUnit;

// Event triggered when a unit is no longer visible to all (due to not being in a vision spheres)
DECLARE_EVENT_OneParam(URTSVisionComponent, FUnitNoVisionOverlap, AUnit*);

/** Holds vision related information and doubles as a collision component to check if units fall withiin the vision radius*/
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) class MYPROJECT_API URTSVisionComponent : public USphereComponent
{
   GENERATED_BODY()

 public:
   /** Since we perform vision calculations on another thread, we need this mutex to prevent data races */
   FWindowsRWLock visionMutex;

   static FUnitNoVisionOverlap unitNoLongerInVisionSpheres;

   // Sets default values for this component's properties
   URTSVisionComponent();

   int  GetVisionCount() const { return enemyVisionCount; }
   void IncVisionCount() { ++enemyVisionCount; }
   void DecVisionCount() { --enemyVisionCount; }

   virtual bool IsVisible() PURE_VIRTUAL(AUnit::IsVisible, return false;);

   /** List of enemies that walked into our vision sphere. Later on we test to see if these units are traceable behind a wall
    * since units won't be visible behind walls.
    */
   TSet<AUnit*> possibleEnemiesInRadius;

   /** Radius of the vision sphere we use to filter what enemies we can see and what corners are included into the vision polygon
    * May be modified (spell might reduce vision or something). Then we'd have to somehow manage the lighting as well...
    */
   UPROPERTY(BlueprintReadOnly, EditAnywhere, Meta = (AllowPrivateAccess = true))
   int visionRadius = 1000;

 protected:
   virtual void BeginPlay() override;

 private:
   /** Tells us how many units have vision on us. When this drops to 0, we are no longer in consideration for a visible enemy.
   * We have to keep track of this since there are multiple units on a team. Even if one unit goes out of range, some other member of the team could
   * be staring down at the unit with this component.
   */
   int enemyVisionCount;

   AUnit* unitOwnerRef;

   UFUNCTION()
   void OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex, bool fromSweep,
                              const FHitResult& sweepRes);

   UFUNCTION()
   void OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};
