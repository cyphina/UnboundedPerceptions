#pragma once

#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "TargetComponent.h"

#include "RTSProjectile.generated.h"

class URTSProjectileStrategy;

/**
 * @brief Base class for all simple projectiles that can be fired in a direction or is homing.
 * TODO: Add some functionality for bullet patterns?
 */
<<<<<<< HEAD
UCLASS()
=======
UCLASS(Abstract)
>>>>>>> componentrefactor
class MYPROJECT_API ARTSProjectile : public AActor
{
   GENERATED_BODY()

 public:
   static ARTSProjectile* MakeRTSProjectile(UWorld* worldToSpawnIn, UTargetComponent* targetComp, FTransform initialTransform = FTransform::Identity,
<<<<<<< HEAD
                                            TSubclassOf<ARTSProjectile>   projectileClass    = ARTSProjectile::StaticClass(),
                                            const URTSProjectileStrategy* projectileStrategy = nullptr);
=======
                                            TSubclassOf<ARTSProjectile> projectileClass    = ARTSProjectile::StaticClass(),
                                            URTSProjectileStrategy*     projectileStrategy = nullptr);
>>>>>>> componentrefactor

   UFUNCTION(BlueprintCallable, Category = "Projectile")
   FORCEINLINE bool IsHoming() { return projectileMovementComponent->bIsHomingProjectile; }

   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   TArray<FGameplayEffectSpecHandle> hitEffects;

 protected:
   virtual void BeginPlay() override;
   virtual void Tick(float DeltaTime) override;

   UFUNCTION(BlueprintImplementableEvent)
   void OnProjectileHitNoDodge(FVector projectileHitLocation);

   UFUNCTION(BlueprintImplementableEvent)
   void OnProjectileDodge(FVector projectileHitLocation);

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
   USphereComponent* collisionComponent;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
   UProjectileMovementComponent* projectileMovementComponent;

<<<<<<< HEAD
   UPROPERTY(EditAnywhere, Category = "Projectile")
=======
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
>>>>>>> componentrefactor
   UStaticMeshComponent* bulletMesh;

 private:
   ARTSProjectile();

   UFUNCTION()
<<<<<<< HEAD
   void OnHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit);
=======
   void OnSweep(UPrimitiveComponent* overlappedComponent, AActor* overlappedActor, UPrimitiveComponent* otherComponent, int32 hitBodyIndex, bool bFromSweep,
                const FHitResult& sweepResult);
>>>>>>> componentrefactor

   void FireInDirection(const FVector& shootDirection) const;

   void FireAtTarget(const AActor* target) const;
};
