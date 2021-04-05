// Created 12/31/20 5:52 PM

#pragma once
#include "GameplayEffectTypes.h"
#include "Engine/DataAsset.h"
#include "RTSProjectileStrategy.generated.h"

/** Sets what kind of targets this bullet can hit */
UENUM(BlueprintType)
<<<<<<< HEAD
enum class EBulletTargetingScheme : uint8 {
=======
enum class EBulletTargetingScheme : uint8
{
>>>>>>> componentrefactor
   Bullet_Ally,  // hits allies
   Bullet_Enemy, // hits enemies
   Bullet_Either // hits both
};

/**
 * @brief Strategy to setup different kinds of common projectile settings \n
 * Instead of having several projectile classes, we have different strategies
 * to simplify the process since most projectiles only differ in strategy and the
 * interface for modifying a data asset is cleaner.
 */
<<<<<<< HEAD
UCLASS()
=======
UCLASS(Blueprintable)
>>>>>>> componentrefactor
class MYPROJECT_API URTSProjectileStrategy : public UDataAsset
{
   GENERATED_BODY()

 public:
<<<<<<< HEAD
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", Meta = (ExposeOnSpawn = "true"))
   UStaticMeshComponent* defaultBulletMesh;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   TArray<FGameplayEffectSpecHandle> defaultHitEffects;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   EBulletTargetingScheme targeting = EBulletTargetingScheme::Bullet_Enemy;

   /** Set this to make bullets go through walls */
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   bool canGoThroughWalls = false;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   bool isHoming = false;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   float bulletSphereRadius = 10.f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   float bulletInitSpeed = 3000.f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
   float bulletMaxSpeed = 3000.f;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
=======
   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Visuals")
   UStaticMesh* defaultBulletMesh;

   UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Visuals")
   UMaterialInterface* defaultBulletMat;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   TArray<FGameplayEffectSpecHandle> defaultHitEffects;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   EBulletTargetingScheme targeting = EBulletTargetingScheme::Bullet_Enemy;

   /** Set this to make bullets go through walls */
   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   bool canGoThroughWalls = false;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   bool isHoming = false;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   bool bIgnoreSelf = true;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   float bulletHomingAcceleration = 25.f;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   float bulletSphereRadius = 50.f;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   float bulletInitSpeed = 3000.f;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
   float bulletMaxSpeed = 3000.f;

   UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
>>>>>>> componentrefactor
   float bulletLifeSpan = 3.f;
};
