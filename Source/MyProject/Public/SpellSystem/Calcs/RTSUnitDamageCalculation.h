// Created 12/23/20 2:35 PM

#pragma once
#include "RTSDamageCalculation.h"
#include "RTSUnitDamageCalculation.generated.h"

UCLASS()
class MYPROJECT_API URTSUnitDamageCalculation : public URTSDamageCalculation
{
   GENERATED_BODY()

 private:
   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
   void DamageTarget(FUpDamage& d, FGameplayTagContainer effects) const override;
};
