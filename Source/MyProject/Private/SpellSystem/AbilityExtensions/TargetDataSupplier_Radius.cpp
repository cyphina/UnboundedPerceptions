// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "TargetDataSupplier_Radius.h"

void ATargetDataSupplier_Radius::ConfirmTargetingAndContinue()
{
   check(ShouldProduceTargetData());
   if (SourceActor) {
      FVector                          Origin = StartLocation.GetTargetingTransform().GetLocation();
      FGameplayAbilityTargetDataHandle Handle = MakeTargetData2(PerformOverlap2(Origin), Origin);
      TargetDataReadyDelegate.Broadcast(Handle);
   }
}

FGameplayAbilityTargetDataHandle ATargetDataSupplier_Radius::MakeTargetData2(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const
{
   if (OwningAbility) {
      /** Use the source location instead of the literal origin */
      return StartLocation.MakeTargetDataHandleFromActors(Actors, false);
   }

   return FGameplayAbilityTargetDataHandle();
}

TArray<TWeakObjectPtr<AActor>> ATargetDataSupplier_Radius::PerformOverlap2(const FVector& Origin)
{
   static FName          RadiusTargetingOverlap = FName(TEXT("RadiusTargetingOverlap"));
   bool                  bTraceComplex          = false;
   FCollisionQueryParams Params(RadiusTargetingOverlap, bTraceComplex);
   Params.bReturnPhysicalMaterial = false;

   TArray<FOverlapResult>      Overlaps;
   FCollisionObjectQueryParams p;
   for(auto traceChannel : traceObjectTypes)
   {
      p.AddObjectTypesToQuery(traceChannel);
   }
   
   SourceActor->GetWorld()->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams(p), FCollisionShape::MakeSphere(Radius), Params);

   TArray<TWeakObjectPtr<AActor>> HitActors;
#if UE_EDITOR
   GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString("Number of Unit Radius Overlaps: ") + FString::FromInt(Overlaps.Num()));
#endif
   for (int32 i = 0; i < Overlaps.Num(); ++i) {
      HitActors.Add(Overlaps[i].GetActor());
      // Should this check to see if these pawns are in the AimTarget list?
      // APawn* PawnActor = Cast<APawn>(Overlaps[i].GetActor());
      // if (PawnActor && !HitActors.Contains(PawnActor) && Filter.FilterPassesForActor(PawnActor))
      {
      }
   }

   return HitActors;
}
