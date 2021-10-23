#include "MyProject.h"
#include "RTSVisionComponent.h"

#include "RTSGameState.h"
#include "Unit.h"

// Sets default values for this component's properties
URTSVisionComponent::URTSVisionComponent()
{
   PrimaryComponentTick.bCanEverTick = true;
   UPrimitiveComponent::SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
   UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   SphereRadius          = 1000.f;
   bUseAttachParentBound = true;
   SetHiddenInGame(true);
   UPrimitiveComponent::SetCollisionObjectType(TRIGGER_CHANNEL);
}

bool URTSVisionComponent::IsUnitVisible() const
{
   if(unitOwnerRef->GetIsEnemy())
   {
      return Cast<ARTSGameState>(GetWorld()->GetGameState())->GetVisibleEnemies().Contains(unitOwnerRef);
   }
   else
   {
      return Cast<ARTSGameState>(GetWorld()->GetGameState())->GetVisiblePlayerUnits().Contains(unitOwnerRef);
   }
}

void URTSVisionComponent::BeginPlay()
{
   Super::BeginPlay();
   unitOwnerRef = Cast<AUnit>(GetOwner());
   SetSphereRadius(visionRadius);
   OnComponentBeginOverlap.AddDynamic(this, &URTSVisionComponent::OnVisionSphereOverlap);
   OnComponentEndOverlap.AddDynamic(this, &URTSVisionComponent::OnVisionSphereEndOverlap);
   if(unitOwnerRef)
   {
      if(!unitOwnerRef->GetIsEnemy())
      {
         SetCollisionProfileName("FriendlyVision");
      }
      else
      {
         SetCollisionProfileName("EnemyVision");
      }
   }
}

void URTSVisionComponent::OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex,
                                                bool fromSweep, const FHitResult& sweepRes)
{
   if(GetOwnerRole() == ROLE_Authority)
   {
      if(AUnit* collidedUnit = Cast<AUnit>(otherActor))
      {
         if(collidedUnit->GetIsEnemy() != Cast<AUnit>(GetOwner())->GetIsEnemy())
         {
            {
               FRWScopeLock(visionMutex, FRWScopeLockType::SLT_Write);
               possibleVisibleEnemies.Add(collidedUnit);
            }
            if(URTSVisionComponent* visionComp = collidedUnit->FindComponentByClass<URTSVisionComponent>())
            {
               visionComp->IncVisionCount();
            }
         }
      }
   }
}

void URTSVisionComponent::OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
   if(GetOwnerRole() == ROLE_Authority)
   {
      if(otherActor)
      {
         if(AUnit* collidedUnit = Cast<AUnit>(otherActor))
         {
            if(AUnit* ownerRef = Cast<AUnit>(GetOwner()))
            {
               if(collidedUnit->GetIsEnemy() != ownerRef->GetIsEnemy())
               {
                  possibleVisibleEnemies.Remove(collidedUnit);
                  URTSVisionComponent* enemyVisionComponent = collidedUnit->FindComponentByClass<URTSVisionComponent>();
                  enemyVisionComponent->DecVisionCount();
               }
            }
         }
      }
   }
}
