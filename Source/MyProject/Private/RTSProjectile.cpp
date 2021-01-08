#include "MyProject.h"
#include "RTSProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RTSProjectileStrategy.h"
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
   collisionComponent->OnComponentHit.AddDynamic(this, &ARTSProjectile::OnHit);

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
                                                  TSubclassOf<ARTSProjectile> projectileClass, const URTSProjectileStrategy* projectileStrategy)
{
   if(!projectileStrategy) {
      projectileStrategy = NewObject<URTSProjectileStrategy>();
   }

   if(AUnit* unitShooter = Cast<AUnit>(targetComp->GetOwner())) {
      ARTSProjectile* projectile                            = worldToSpawnIn->SpawnActorDeferred<ARTSProjectile>(projectileClass, initialTransform);
      projectile->projectileMovementComponent->InitialSpeed = projectileStrategy->bulletInitSpeed;
      projectile->projectileMovementComponent->MaxSpeed     = projectileStrategy->bulletMaxSpeed;
      projectile->InitialLifeSpan                           = projectileStrategy->bulletLifeSpan;

      projectile->collisionComponent->InitSphereRadius(projectileStrategy->bulletSphereRadius);
      projectile->hitEffects.Append(projectileStrategy->defaultHitEffects);
      if(!projectile->bulletMesh) {
         if(projectileStrategy->defaultBulletMesh) projectile->bulletMesh = projectileStrategy->defaultBulletMesh;
      }

      switch(projectileStrategy->targeting) {
         case EBulletTargetingScheme::Bullet_Ally: {
            if(unitShooter->GetIsEnemy()) {
               projectile->collisionComponent->SetCollisionResponseToChannel(ENEMY_CHANNEL, ECR_Block);
            } else {
               projectile->collisionComponent->SetCollisionResponseToChannel(FRIENDLY_CHANNEL, ECR_Block);
            }
            break;
         }
         case EBulletTargetingScheme::Bullet_Enemy: {
            if(unitShooter->GetIsEnemy()) {
               projectile->collisionComponent->SetCollisionResponseToChannel(FRIENDLY_CHANNEL, ECR_Block);
            } else {
               projectile->collisionComponent->SetCollisionResponseToChannel(ENEMY_CHANNEL, ECR_Block);
            }
            break;
         }
         case EBulletTargetingScheme::Bullet_Either:
            projectile->collisionComponent->SetCollisionResponseToChannel(ENEMY_CHANNEL, ECR_Block);
            projectile->collisionComponent->SetCollisionResponseToChannel(FRIENDLY_CHANNEL, ECR_Block);
            break;
      }

      if(projectileStrategy->canGoThroughWalls) {
         projectile->collisionComponent->SetCollisionResponseToChannel(VISION_BLOCKER_CHANNEL, ECR_Ignore);
         projectile->collisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);
      } else {
         projectile->collisionComponent->SetCollisionResponseToChannel(VISION_BLOCKER_CHANNEL, ECR_Block);
         projectile->collisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
      }

      projectile->FinishSpawning(initialTransform);

      if(projectileStrategy->isHoming)
         projectile->FireAtTarget(targetComp->GetTargetActorOrUnit());
      else
         projectile->FireInDirection((targetComp->GetTargetActorOrUnit()->GetActorLocation() -
                                      FVector(initialTransform.GetLocation().X, initialTransform.GetLocation().Y, initialTransform.GetLocation().Z))
                                         .GetSafeNormal());
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
   projectileMovementComponent->bIsHomingProjectile   = true;
   projectileMovementComponent->HomingTargetComponent = target->GetRootComponent();
}

void ARTSProjectile::OnHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit)
{
   UAbilitySystemComponent* collidedActorAbilityComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(otherActor);
   if(collidedActorAbilityComp) {
      for(FGameplayEffectSpecHandle eff : hitEffects) {
         collidedActorAbilityComp->ApplyGameplayEffectSpecToSelf(*eff.Data.Get());
      }
   }
}
