// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "GAbilityFHeroTrace.h"

FHitResult AGAbilityFHeroTrace::PerformTrace(AActor* InSourceActor)
{
   static const FName LineTraceSingleName(TEXT("AGameplayAbilityTargetActor_HeroTrace"));
   bool               bTraceComplex = false;
   TArray<AActor*>    ActorsToIgnore;

   ActorsToIgnore.Add(InSourceActor);
   // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Hit From Hero Trace!"));
   FCollisionQueryParams Params(LineTraceSingleName, bTraceComplex);
   Params.bReturnPhysicalMaterial = true;
   Params.bTraceAsyncScene        = true;
   Params.AddIgnoredActors(ActorsToIgnore);

   FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation(); // InSourceActor->GetActorLocation();
   FVector TraceEnd   = TraceStart + InSourceActor->GetActorForwardVector() * MaxRange;

   FHitResult ReturnHitResult;
   LineTraceWithFilter(ReturnHitResult, InSourceActor->GetWorld(), Filter, TraceStart, TraceEnd, TraceProfile.Name, Params);
   // Default to end of trace line if we don't hit anything.
   if (!ReturnHitResult.bBlockingHit) { ReturnHitResult.Location = TraceEnd; }
   if (AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActor.Get()) {
      const bool    bHitActor       = (ReturnHitResult.bBlockingHit && (ReturnHitResult.Actor != NULL));
      const FVector ReticleLocation = (bHitActor && LocalReticleActor->bSnapToTargetedActor) ? ReturnHitResult.Actor->GetActorLocation() : ReturnHitResult.Location;

      LocalReticleActor->SetActorLocation(ReticleLocation);
      LocalReticleActor->SetIsTargetAnActor(bHitActor);
   }

#if ENABLE_DRAW_DEBUG
   if (bDebug) {
      DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green);
      DrawDebugSphere(GetWorld(), TraceEnd, 100.0f, 16, FColor::Green);
   }
#endif // ENABLE_DRAW_DEBUG
   return ReturnHitResult;
}
