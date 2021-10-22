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
UCLASS(Abstract, Blueprintable, BlueprintType)
class MYPROJECT_API ARTSProjectile : public AActor
{
   GENERATED_BODY()

 public:
   static ARTSProjectile* MakeRTSProjectile(UWorld* worldToSpawnIn, UTargetComponent* targetComp, FTransform initialTransform = FTransform::Identity,
                                            TSubclassOf<ARTSProjectile> projectileClass    = ARTSProjectile::StaticClass(),
                                            URTSProjectileStrategy*     projectileStrategy = nullptr);

   UFUNCTION(BlueprintCallable, Category = "Projectile")
   FORCEINLINE bool IsHoming() { return projectileMovementComponent->bIsHomingProjectile; }

   UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Projectile")
   TArray<TEnumAsByte<EObjectTypeQuery>> GetHittableObjectTypes();

   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   TArray<FGameplayEffectSpecHandle> hitEffects;

 protected:
   virtual void BeginPlay() override;
   virtual void Tick(float DeltaTime) override;

   UFUNCTION(BlueprintImplementableEvent)
   void OnProjectileHitNoDodge(FVector projectileHitLocation);

   UFUNCTION(BlueprintImplementableEvent)
   void OnProjectileDodge(FVector projectileHitLocation);

   /** Only valid if the bullet hits a target successfully and it does not dodge. Use within OnProjectileHitNoDodge */
   UFUNCTION(BlueprintCallable, BlueprintPure)
   AActor* GetHitTarget() const { return hitTarget; }

   UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
   USphereComponent* collisionComponent;

   UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
   UProjectileMovementComponent* projectileMovementComponent;

   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
   UStaticMeshComponent* bulletMesh;

   AActor* hitTarget;

 private:
   ARTSProjectile();

   UFUNCTION()
   void OnSweep(UPrimitiveComponent* overlappedComponent, AActor* overlappedActor, UPrimitiveComponent* otherComponent, int32 hitBodyIndex, bool bFromSweep,
                const FHitResult& sweepResult);

   void FireInDirection(const FVector& shootDirection) const;

   void FireAtTarget(const AActor* target) const;
};
