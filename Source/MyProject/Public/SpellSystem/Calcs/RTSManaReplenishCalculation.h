// Created 3/15/21 5:10 AM

#pragma once
#include "GameplayEffectExecutionCalculation.h"
#include "RTSManaReplenishCalculation.generated.h"

/**
 * @brief Handles healing and replenishing mana. Does not handle manaburn nor damage, so don't try to invert the numbers to hack a solution together.
 */
UCLASS()
class MYPROJECT_API URTSManaReplenishCalculation : public UGameplayEffectExecutionCalculation
{
   GENERATED_BODY()

 public:
   URTSManaReplenishCalculation();

 protected:
   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};
