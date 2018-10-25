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
         collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Block); // Friendly
         break;
      case EBulletTargettingScheme::Bullet_Enemy:
         collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); // Enemy
         break;
      case EBulletTargettingScheme::Bullet_Either:
         collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block); // Enemy
         collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECollisionResponse::ECR_Block); // Friendly
         break;
   }

   if (canGoThroughWalls) {
      collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Ignore); // vision blocker
      collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);       // static mesh
   } else {
      collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel6, ECollisionResponse::ECR_Block); // vision blocker
      collisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);       // static mesh
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
