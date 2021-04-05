#include "MyProject.h"
#include "RTSProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RTSDamageEffect.h"
#include "RTSProjectileStrategy.h"
#include "SpellFunctionLibrary.h"
#include "TargetComponent.h"
#include "Unit.h"

ARTSProjectile::ARTSProjectile()
{
   PrimaryActorTick.bCanEverTick = true;
   collisionComponent            = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
   RootComponent                 = collisionComponent;

   projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
   projectileMovementComponent->SetUpdatedComponent(collisionComponent);

   projectileMovementComponent->bRotationFollowsVelocity = true;
   projectileMovementComponent->bShouldBounce            = false;

   collisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
   collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ARTSProjectile::OnSweep);

   bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
   bulletMesh->SetupAttachment(RootComponent);
}

void ARTSProjectile::BeginPlay()
{
   Super::BeginPlay();
   collisionComponent->IgnoreActorWhenMoving(Cast<AActor>(GetOwner()), true);
}

void ARTSProjectile::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}

ARTSProjectile* ARTSProjectile::MakeRTSProjectile(UWorld* worldToSpawnIn, UTargetComponent* targetComp, FTransform initialTransform,
                                                  TSubclassOf<ARTSProjectile> projectileClass, URTSProjectileStrategy* projectileStrategy)
{
   if(AUnit* unitShooter = Cast<AUnit>(targetComp->GetOwner()))
   {
      ARTSProjectile* projectile = worldToSpawnIn->SpawnActorDeferred<ARTSProjectile>(projectileClass, initialTransform, unitShooter);

      if(!projectileStrategy)
      {
         projectileStrategy = URTSProjectileStrategy::StaticClass()->GetDefaultObject<URTSProjectileStrategy>();
      }

      if(projectileStrategy->bIgnoreSelf)
      {
         projectile->collisionComponent->IgnoreActorWhenMoving(unitShooter, true);
      }

      projectile->projectileMovementComponent->InitialSpeed                = projectileStrategy->bulletInitSpeed;
      projectile->projectileMovementComponent->MaxSpeed                    = projectileStrategy->bulletMaxSpeed;
      projectile->projectileMovementComponent->HomingAccelerationMagnitude = projectileStrategy->bulletHomingAcceleration;
      projectile->InitialLifeSpan                                          = projectileStrategy->bulletLifeSpan;

      projectile->collisionComponent->InitSphereRadius(projectileStrategy->bulletSphereRadius);

      const static float INIT_BULLET_RADIUS = 50.f;
      projectile->SetActorScale3D(FVector(projectileStrategy->bulletSphereRadius / INIT_BULLET_RADIUS));
      projectile->hitEffects.Append(projectileStrategy->defaultHitEffects);

      if(projectile->bulletMesh)
      {
         if(projectileStrategy->defaultBulletMesh)
         {
            projectile->bulletMesh->SetStaticMesh(projectileStrategy->defaultBulletMesh);
         }

         if(projectileStrategy->defaultBulletMat)
         {
            projectile->bulletMesh->SetMaterial(0, projectileStrategy->defaultBulletMat);
         }
      }

      switch(projectileStrategy->targeting)
      {
         case EBulletTargetingScheme::Bullet_Ally:
         {
            if(unitShooter->GetIsEnemy())
            {
               projectile->collisionComponent->SetCollisionResponseToChannel(ENEMY_OBJECT_CHANNEL, ECR_Block);
            }
            else
            {
               projectile->collisionComponent->SetCollisionResponseToChannel(ALLY_OBJECT_CHANNEL, ECR_Block);
            }
            break;
         }
         case EBulletTargetingScheme::Bullet_Enemy:
         {
            if(unitShooter->GetIsEnemy())
            {
               projectile->collisionComponent->SetCollisionResponseToChannel(ALLY_OBJECT_CHANNEL, ECR_Block);
            }
            else
            {
               projectile->collisionComponent->SetCollisionResponseToChannel(ENEMY_OBJECT_CHANNEL, ECR_Block);
            }
            break;
         }
         case EBulletTargetingScheme::Bullet_Either:
         {
            projectile->collisionComponent->SetCollisionResponseToChannel(ENEMY_OBJECT_CHANNEL, ECR_Block);
            projectile->collisionComponent->SetCollisionResponseToChannel(ALLY_OBJECT_CHANNEL, ECR_Block);
            break;
         }
      }

      if(projectileStrategy->canGoThroughWalls)
      {
         projectile->collisionComponent->SetCollisionResponseToChannel(VISION_BLOCKER_CHANNEL, ECR_Ignore);
         projectile->collisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
      }
      else
      {
         projectile->collisionComponent->SetCollisionResponseToChannel(VISION_BLOCKER_CHANNEL, ECR_Block);
         projectile->collisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
      }

      projectile->FinishSpawning(initialTransform);

      if(projectileStrategy->isHoming)
      {
         projectile->FireAtTarget(targetComp->GetTargetActorOrUnit());
      }
      else
      {
         projectile->FireInDirection((targetComp->GetTargetActorOrUnit()->GetActorLocation() -
                                      FVector(initialTransform.GetLocation().X, initialTransform.GetLocation().Y, initialTransform.GetLocation().Z))
                                         .GetSafeNormal());
      }
      return projectile;
   }
   return nullptr;
}

void ARTSProjectile::FireInDirection(const FVector& shootDirection) const
{
   projectileMovementComponent->Velocity = shootDirection * projectileMovementComponent->InitialSpeed;
}

void ARTSProjectile::FireAtTarget(const AActor* target) const
{
   if(target)
   {
      projectileMovementComponent->bIsHomingProjectile   = true;
      projectileMovementComponent->HomingTargetComponent = target->GetRootComponent();
   }
}

void ARTSProjectile::OnSweep(UPrimitiveComponent* overlappedComponent, AActor* overlappedActor, UPrimitiveComponent* otherComponent, int32 hitBodyIndex, bool bFromSweep,
                             const FHitResult& sweepResult)
{
   UAbilitySystemComponent* collidedActorAbilityComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(overlappedActor);
   if(collidedActorAbilityComp)
   {
      const FGameplayEffectSpecHandle* damageEffect = hitEffects.FindByPredicate([](const FGameplayEffectSpecHandle& effectHandle) {
         if(effectHandle.Data->Def->GetClass()->IsChildOf(URTSDamageEffect::StaticClass()))
         {
            return true;
         }
         return false;
      });

      // REMARK: Note that projectiles without damage are at the mercy of hitting depending on the very last attack done without damage, so for now all projectils should have a little damage
      if(damageEffect)
      {
         collidedActorAbilityComp->ApplyGameplayEffectSpecToSelf(*damageEffect->Data.Get());
         hitEffects.RemoveSingle(*damageEffect);
      }

      if(AUnit* unitShooter = Cast<AUnit>(GetOwner()))
      {
         if(!unitShooter->GetCombatInfo()->bMissLastHit)
         {
            for(FGameplayEffectSpecHandle eff : hitEffects)
            {
               collidedActorAbilityComp->ApplyGameplayEffectSpecToSelf(*eff.Data.Get());
            }

            OnProjectileHitNoDodge(sweepResult.Location);

            GetWorld()->GetTimerManager().SetTimerForNextTick([this]() {
               Destroy();
            });
         }
         else
         {
            OnProjectileDodge(sweepResult.Location);
         }
      }
   }
}
