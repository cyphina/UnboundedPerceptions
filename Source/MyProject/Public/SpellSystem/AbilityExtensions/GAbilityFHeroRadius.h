// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Radius.h"
#include "GAbilityFHeroRadius.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API AGAbilityFHeroRadius : public AGameplayAbilityTargetActor_Radius
{
   GENERATED_BODY()

 public:
   virtual void ConfirmTargetingAndContinue() override;

 private:
   TArray<TWeakObjectPtr<AActor>>   PerformOverlap2(const FVector& Origin);
   FGameplayAbilityTargetDataHandle MakeTargetData2(const TArray<TWeakObjectPtr<AActor>>& Actors, const FVector& Origin) const;
};
