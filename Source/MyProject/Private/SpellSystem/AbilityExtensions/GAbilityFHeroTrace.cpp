// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "GAbilityFHeroTrace.h"

TArray<TWeakObjectPtr<AActor>> AGAbilityFHeroTrace::PerformTrace(AActor* InSourceActor)
{
   static const FName LineTraceSingleName(TEXT("AGameplayAbilityTargetActor_HeroTrace"));
   bool               bTraceComplex = false;
   TArray<AActor*>    ActorsToIgnore;

   ActorsToIgnore.Add(InSourceActor);
   // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, TEXT("Hit From Hero Trace!"));
   FCollisionQueryParams Params(LineTraceSingleName, bTraceComplex);
   Params.bReturnPhysicalMaterial = true;
   Params.AddIgnoredActors(ActorsToIgnore);

   FVector TraceStart = StartLocation.GetTargetingTransform().GetLocation(); // InSourceActor->GetActorLocation();
   FVector TraceEnd   = TraceStart + InSourceActor->GetActorForwardVector() * maxRange;

   TArray<FHitResult> ReturnHitResults;

  
   InSourceActor->GetWorld()->SweepMultiByChannel(ReturnHitResults, TraceStart, TraceEnd, FQuat::Identity, traceChannel,
                                                  FCollisionShape::MakeCapsule(radius, halfHeight), Params);

   /*if(AGameplayAbilityWorldReticle* LocalReticleActor = ReticleActor.Get())
   {
      const bool    bHitActor       = (ReturnHitResult.bBlockingHit && (ReturnHitResult.Actor != NULL));
      const FVector ReticleLocation = (bHitActor && LocalReticleActor->bSnapToTargetedActor) ? ReturnHitResult.Actor->GetActorLocation() : ReturnHitResult.Location;

      LocalReticleActor->SetActorLocation(ReticleLocation);
      LocalReticleActor->SetIsTargetAnActor(bHitActor);
   }*/

#if ENABLE_DRAW_DEBUG
   if(bDebug)
   {
      DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.f);
      const FVector capsuleLocation = ReturnHitResults.Num() > 0 ? ReturnHitResults[ReturnHitResults.Num() - 1].ImpactPoint : TraceEnd;
      DrawDebugCapsule(GetWorld(), capsuleLocation, halfHeight, radius, FQuat::Identity, FColor::Green, false, 2.f);
   }
#endif // ENABLE_DRAW_DEBUG

   TArray<TWeakObjectPtr<AActor>> TargetDataActors;
   for(const FHitResult& hitResult : ReturnHitResults)
   {
      TargetDataActors.Add(hitResult.Actor);
   }
   return TargetDataActors;
}

void AGAbilityFHeroTrace::ConfirmTargetingAndContinue()
{
   if(ShouldProduceTargetData())
   {
      FVector Origin = StartLocation.GetTargetingTransform().GetLocation();
      if(OwningAbility)
      {
         const FGameplayAbilityTargetDataHandle Handle = StartLocation.MakeTargetDataHandleFromActors(PerformTrace(StartLocation.SourceActor), false);
         TargetDataReadyDelegate.Broadcast(Handle);
      }
   }
}
