// Created 3/15/21 5:10 AM

#pragma once
#include "GameplayEffectExecutionCalculation.h"
#include "RTSVitalReplenishCalculation.generated.h"

UCLASS()
class MYPROJECT_API URTSVitalReplenishCalculation : public UGameplayEffectExecutionCalculation
{
   GENERATED_BODY()

 public:
   URTSVitalReplenishCalculation();

 protected:
   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};
