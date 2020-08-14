// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "RTSVisionComponent.h"

#include "Unit.h"

// Sets default values for this component's properties
URTSVisionComponent::URTSVisionComponent()
{
   PrimaryComponentTick.bCanEverTick = true;
   SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
   SetCollisionEnabled(ECollisionEnabled::QueryOnly);
   bUseAttachParentBound = true;
   SetCollisionObjectType(TRIGGER_CHANNEL);
}

void URTSVisionComponent::BeginPlay()
{
   Super::BeginPlay();
   OnComponentBeginOverlap.AddDynamic(this, &URTSVisionComponent::OnVisionSphereOverlap);

   SetSphereRadius(visionRadius);
}

void URTSVisionComponent::OnVisionSphereOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int otherBodyIndex,
                                                bool fromSweep, const FHitResult& sweepRes)
{
   // On the server check to see if the unit we collided with is an enemy (relative to this unit).
   // If so, we have to record it as one of the possible units we trace to for wall checks
   if(GetOwnerRole() == ROLE_Authority) {
      if(otherActor->GetClass()->IsChildOf(AUnit::StaticClass())) {
         AUnit* collidedUnit = Cast<AUnit>(otherActor);
         {
            if(collidedUnit->GetIsEnemy() != unitOwnerRef->GetIsEnemy()) {
               possibleEnemiesInRadius.Add(collidedUnit);
               collidedUnit->FindComponentByClass<URTSVisionComponent>()->IncVisionCount();
            }
         }
      }
   }
}

void URTSVisionComponent::OnVisionSphereEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
   if(GetOwnerRole() == ROLE_Authority) {
      if(otherActor->GetClass()->IsChildOf(AUnit::StaticClass())) {
         AUnit* collidedUnit = Cast<AUnit>(otherActor);
         if(collidedUnit->GetIsEnemy() != unitOwnerRef->GetIsEnemy()) {
            possibleEnemiesInRadius.Remove(collidedUnit);
            auto enemyVisionComponent = collidedUnit->FindComponentByClass<URTSVisionComponent>() enemyVisionComponent->DecVisionCount();

            // If there's no longer any units watching this enemy unit
            if(!enemyVisionComponent->GetVisionCount()) {
               collidedUnit->GetCapsuleComponent()->SetVisibility(false, true);
               collidedUnit->GetCapsuleComponent()->SetCollisionResponseToChannel(SELECTABLE_BY_CLICK_CHANNEL, ECR_Ignore);

               unitNoLongerInVisionSpheres.Broadcast(collidedUnit);
            }
         }
      }
   }
}
