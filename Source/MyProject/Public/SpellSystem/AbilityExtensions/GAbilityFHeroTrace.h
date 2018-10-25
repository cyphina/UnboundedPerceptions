// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "GAbilityFHeroTrace.generated.h"

/**
 *
 */
UCLASS()
class MYPROJECT_API AGAbilityFHeroTrace : public AGameplayAbilityTargetActor_Trace
{
   GENERATED_BODY()
 protected:
   virtual FHitResult PerformTrace(AActor* InSourceActor) override;
};
