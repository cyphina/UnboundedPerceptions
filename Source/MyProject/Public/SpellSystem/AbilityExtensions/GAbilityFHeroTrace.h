#pragma once

#include "Abilities/GameplayAbilityTargetActor_Trace.h"
#include "GAbilityFHeroTrace.generated.h"

/**
 * Traces a capsule in front of the hero up to the specified range.
 */
UCLASS(Blueprintable)
class MYPROJECT_API AGAbilityFHeroTrace : public AGameplayAbilityTargetActor
{
   GENERATED_BODY()

 protected:
   virtual void ConfirmTargetingAndContinue() override;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
   float maxRange;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
   float radius;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
   float halfHeight;

   UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
   TEnumAsByte<ECollisionChannel> traceChannel;

 private:
   virtual TArray<TWeakObjectPtr<AActor>> PerformTrace(AActor* InSourceActor);
};
