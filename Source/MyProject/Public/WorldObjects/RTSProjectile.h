// Fill out your copyright notice in the Description page of Project Settings.

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
UCLASS()
class MYPROJECT_API ARTSProjectile : public AActor
{
   GENERATED_BODY()

 public:
   static ARTSProjectile* MakeRTSProjectile(UWorld* worldToSpawnIn, UTargetComponent* targetComp, FTransform initialTransform = FTransform::Identity,
                                            TSubclassOf<ARTSProjectile>         projectileClass         = ARTSProjectile::StaticClass(),
                                            URTSProjectileStrategy* projectileStrategy = nullptr);
                                            

   UFUNCTION(BlueprintCallable, Category = "Projectile")
   FORCEINLINE bool IsHoming() { return projectileMovementComponent->bIsHomingProjectile; }

   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   TArray<FGameplayEffectSpecHandle> hitEffects;

 protected:
   virtual void BeginPlay() override;
   virtual void Tick(float DeltaTime) override;

   UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
   USphereComponent* collisionComponent;

   UPROPERTY(VisibleAnywhere, Category = "Movement")
   UProjectileMovementComponent* projectileMovementComponent;

   UPROPERTY(EditAnywhere, Category = "Projectile")
   UStaticMeshComponent* bulletMesh;

 private:
   ARTSProjectile();

   UFUNCTION()
   void OnHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit);

   void FireInDirection(const FVector& shootDirection) const;

   void FireAtTarget(const AActor* target) const;
};
