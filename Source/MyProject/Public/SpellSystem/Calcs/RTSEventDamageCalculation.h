// Created 12/23/20 4:31 AM

#pragma once
#include "RTSDamageCalculation.h"
#include "SpellSystem/DamageStructs.h"
#include "RTSEventDamageCalculation.generated.h"

UCLASS()
class MYPROJECT_API URTSEventDamageCalculation : public URTSDamageCalculation
{
   GENERATED_BODY()

 private:
   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams,
                               OUT FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
	
   UFUNCTION(BlueprintCallable)
   void DamageTarget(UPARAM(ref) FUpDamage& d, FGameplayTagContainer effects) const override;
};
