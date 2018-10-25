// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "RTSProjectile.generated.h"

/**Sets what kind of targets this bullet can hit*/
UENUM(BlueprintType)
enum class EBulletTargettingScheme : uint8 {
   Bullet_Ally,  // hits allies
   Bullet_Enemy, // hits enemies
   Bullet_Either // hits both
};

UCLASS()
class MYPROJECT_API ARTSProjectile : public AActor
{
   GENERATED_BODY()

   UFUNCTION()
   void OnHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit);

   void SetBulletTargetting();

 public:
   ARTSProjectile();
   virtual void BeginPlay() override;
   virtual void Tick(float DeltaTime) override;

   UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
   USphereComponent* collisionComponent;

   UPROPERTY(VisibleAnywhere, Category = "Movement")
   UProjectileMovementComponent* projectileMovementComponent;

   UPROPERTY(VisibleAnywhere, Category = "Projectile")
   UStaticMeshComponent* bulletMesh;

   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   TArray<FGameplayEffectSpecHandle> hitEffects;

   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   EBulletTargettingScheme targetting;

   /**Set this to make bullets go through walls*/
   UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   bool canGoThroughWalls = false;

   UFUNCTION(BlueprintCallable, Category = "Projectile")
   void FireInDirection(const FVector& shootDirection);

   UFUNCTION(BlueprintCallable, Category = "Projectile")
   void FireAtTarget(const AActor* target);
};
