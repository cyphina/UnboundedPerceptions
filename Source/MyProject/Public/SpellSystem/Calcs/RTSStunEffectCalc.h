#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "RTSStunEffectCalc.generated.h"

/**
 * This execution simply sends an AI message to the controller of the stunned unit so the AI system knows to not wait for any current actions but to recalculate
 * the thought process since we probably don't want them to make the same decision after we are stunned.
 */
UCLASS()
class MYPROJECT_API URTSStunEffectCalc : public UGameplayEffectExecutionCalculation
{
   GENERATED_BODY()

   void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& executionParams, OUT FGameplayEffectCustomExecutionOutput& outExecutionOutput) const override;
};
