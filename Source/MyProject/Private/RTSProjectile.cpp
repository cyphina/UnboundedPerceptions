#include "MyProject.h"
#include "RTSProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
ARTSProjectile::ARTSProjectile()
{
   PrimaryActorTick.bCanEverTick = true;
   collisionComponent            = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
   collisionComponent->InitSphereRadius(10.f);
   RootComponent = collisionComponent;

   projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComponent");
   projectileMovementComponent->SetUpdatedComponent(collisionComponent);
   projectileMovementComponent->InitialSpeed             = 3000.0f;
   projectileMovementComponent->MaxSpeed                 = 3000.f;
   projectileMovementComponent->bRotationFollowsVelocity = true;
   projectileMovementComponent->bShouldBounce            = false;
   InitialLifeSpan                                       = 3.0f;

   collisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
   collisionComponent->OnComponentHit.AddDynamic(this, &ARTSProjectile::OnHit);

   bulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
   bulletMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARTSProjectile::BeginPlay()
{
   Super::BeginPlay();
   switch (targetting) {
      case EBulletTargettingScheme::Bullet_Ally:
         collisionComponent->SetCollisionResponseToChannel(FRIENDLY_CHANNEL, ECR_Block);
         break;
      case EBulletTargettingScheme::Bullet_Enemy:
         collisionComponent->SetCollisionResponseToChannel(ENEMY_CHANNEL, ECR_Block); 
         break;
      case EBulletTargettingScheme::Bullet_Either:
         collisionComponent->SetCollisionResponseToChannel(ENEMY_CHANNEL, ECR_Block); 
         collisionComponent->SetCollisionResponseToChannel(FRIENDLY_CHANNEL, ECR_Block); 
         break;
   }

   if (canGoThroughWalls) {
      collisionComponent->SetCollisionResponseToChannel(VISION_BLOCKER_CHANNEL, ECR_Ignore); 
      collisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Ignore);       
   } else {
      collisionComponent->SetCollisionResponseToChannel(VISION_BLOCKER_CHANNEL, ECR_Block); 
      collisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);       
   }

   collisionComponent->IgnoreActorWhenMoving(Cast<AActor>(GetOwner()), true);
}

// Called every frame
void ARTSProjectile::Tick(float DeltaTime)
{
   Super::Tick(DeltaTime);
}

void ARTSProjectile::FireInDirection(const FVector& shootDirection)
{
   projectileMovementComponent->Velocity = shootDirection * projectileMovementComponent->InitialSpeed;
}

void ARTSProjectile::FireAtTarget(const AActor* target)
{
   projectileMovementComponent->bIsHomingProjectile   = true;
   projectileMovementComponent->HomingTargetComponent = target->GetRootComponent();
}

void ARTSProjectile::OnHit(UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit)
{
   UAbilitySystemComponent* abilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(otherActor);
   if (abilitySystemComp) {
      for (FGameplayEffectSpecHandle eff : hitEffects) {
         abilitySystemComp->ApplyGameplayEffectSpecToSelf(*eff.Data.Get());
      }
   }
}
