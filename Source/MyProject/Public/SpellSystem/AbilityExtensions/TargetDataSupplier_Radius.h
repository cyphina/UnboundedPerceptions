// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Radius.h"
#include "TargetDataSupplier_Radius.generated.h"

/**
 * Traces around the starting point like AGameplayAbilityTargetActor_Radius, but has a tracing filter
 */
UCLASS()
class MYPROJECT_API ATargetDataSupplier_Radius : public AGameplayAbilityTargetActor_Radius
{
   GENERATED_BODY()

 public:
   virtual void ConfirmTargetingAndContinue() override;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = TargetParams)
   TArray<TEnumAsByte<ECollisionChannel>> traceObjectTypes;

 private:
   TArray<TWeakObjectPtr<AActor>>   PerformOverlap2(const FVector& Origin);
   FGameplayAbilityTargetDataHandle MakeTargetData2(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const;
};
